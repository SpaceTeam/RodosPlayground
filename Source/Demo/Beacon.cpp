#include "Topics.hpp"

#include <rodos.h>

#include <etl/bitset.h>
#include <etl/string.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <stm32f4xx.h>
#include <stm32f4xx_pwr.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_rtc.h>

// Beacon Structure : see https://wiki.tust.at/books/sts1/page/uart-commands-for-pcb-v1
// 1.   Timestamp
// 2.   Reset Counter
// 3.   Edu Heartbeat
// 4.   GPIO bitfield

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}

namespace rpg
{
constexpr auto pc14 = GPIO_046;  // PC14
constexpr auto pc15 = GPIO_047;  // PC15
constexpr auto pb1 = GPIO_017;   // PB1

auto eduUpdateGpio = HAL_GPIO(pb1);
auto epsChargingGpio = HAL_GPIO(pc14);
auto epsBatteryGoodGpio = HAL_GPIO(pc15);

auto eduIsAliveBuffer = CommBuffer<int32_t>();
auto eduIsAliveSubscriber = Subscriber(eduIsAliveTopic, eduIsAliveBuffer, "eduIsAliveSubscriber");


auto CreateGpioBitfield(bool epsIsCharging, bool epsBatteryIsGood, bool eduHasUpdate)
{
  constexpr auto nBits = sizeof(int32_t) * 8;
  auto gpioBitField = etl::bitset<nBits>();
  gpioBitField.set(0, epsIsCharging);
  gpioBitField.set(1, epsBatteryIsGood);
  gpioBitField.set(2, eduHasUpdate);
  return static_cast<int32_t>(gpioBitField.to_ulong());
}


auto CreateBeacon(int32_t timestamp,
                  int32_t resetCounter,
                  int32_t eduIsAlive,
                  int32_t gpioBitfield)
{
  constexpr auto startByte = '?';
  uint8_t checksum = 0;
  constexpr auto beaconSize = sizeof(startByte) + sizeof(timestamp) + sizeof(resetCounter)
                            + sizeof(eduIsAlive) + sizeof(gpioBitfield) + sizeof(checksum);

  // TODO: Use std::array of uint8_t or std::byte
  auto beacon = etl::string<beaconSize + 1>();
  beacon.initialize_free_space();

  size_t i = 0;
  beacon[i] = startByte;
  i += sizeof(startByte);
  // TODO: Add a function for this: serializeTo, copyTo, writeTo, ... increment the pointer instead
  // of index? Make the index an in/out parameter?
  std::memcpy(&beacon[i], &timestamp, sizeof(timestamp));
  i += sizeof(timestamp);
  std::memcpy(&beacon[i], &resetCounter, sizeof(resetCounter));
  i += sizeof(resetCounter);
  std::memcpy(&beacon[i], &eduIsAlive, sizeof(eduIsAlive));
  i += sizeof(eduIsAlive);
  std::memcpy(&beacon[i], &gpioBitfield, sizeof(gpioBitfield));
  i += sizeof(gpioBitfield);

  beacon.uninitialized_resize(beaconSize);

  checksum =
    static_cast<uint8_t>(std::accumulate(std::begin(beacon) + 1, std::end(beacon) - 1, 0) & 0xFF);
  std::memcpy(&beacon[i], &checksum, sizeof(checksum));
  return beacon;
}


class BeaconThread : public StaticThread<>
{
  void init() override
  {
    // 1:  Enable the power controller APB1 interface
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    // 2: Enable access to RTC domain using the PWR_BackupAccessCmd() function.
    PWR_BackupAccessCmd(ENABLE);

    epsChargingGpio.init(/*isOutput=*/false, 1, 0);
    epsBatteryGoodGpio.init(/*isOutput=*/false, 1, 0);
    eduUpdateGpio.init(/*isOutput=*/false, 1, 0);
  }


  void run() override
  {
    // TODO: Move to init()
    auto resetCounter = RTC_ReadBackupRegister(RTC_BKP_DR0);
    ++resetCounter;
    RTC_WriteBackupRegister(RTC_BKP_DR0, resetCounter);

    TIME_LOOP(0, 2000 * MILLISECONDS)
    {
      auto const timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);
      auto const epsIsCharging = static_cast<bool>(epsChargingGpio.readPins());
      auto const epsBatteryIsGood = static_cast<bool>(epsBatteryGoodGpio.readPins());
      auto const eduHasUpdate = static_cast<bool>(eduUpdateGpio.readPins());
      auto gpioBitField = CreateGpioBitfield(epsIsCharging, epsBatteryIsGood, eduHasUpdate);
      // Get the lastet value
      int32_t eduIsAlive = 0;
      eduIsAliveBuffer.get(eduIsAlive);

      auto beacon = CreateBeacon(
        timestamp, static_cast<int32_t>(resetCounter), eduIsAlive, gpioBitField);
      for(auto c : beacon)
      {
        uart_stdout.putcharNoWait(c);
      }
    }
  }
};


auto const beaconThread = BeaconThread();
}
