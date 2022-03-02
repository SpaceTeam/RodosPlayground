#include "Topics.hpp"

#include <rodos.h>

#include <etl/bitset.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include <stm32f4xx.h>
#include <stm32f4xx_pwr.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_rtc.h>


// Beacon Structure : see https://wiki.tust.at/books/sts1/page/uart-commands-for-pcb-v1
// 1.   Timestamp
// 2.   Reset Counter
// 3.   Edu Heartbeat
// 4.   GPIO bit field

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

auto eduIsAliveBuffer = CommBuffer<bool>();
auto eduIsAliveSubscriber = Subscriber(eduIsAliveTopic, eduIsAliveBuffer, "eduIsAliveSubscriber");


auto CreateGpioBitField(bool epsIsCharging, bool epsBatteryIsGood, bool eduHasUpdate)
{
  constexpr auto nBits = sizeof(int32_t) * 8;
  auto gpioBitField = etl::bitset<nBits>();
  gpioBitField.set(0, epsIsCharging);
  gpioBitField.set(1, epsBatteryIsGood);
  gpioBitField.set(2, eduHasUpdate);
  return static_cast<int32_t>(gpioBitField.to_ulong());
}


auto CopyTo(std::span<std::byte> buffer, std::size_t * const position, auto value)
{
  auto newPosition = *position + sizeof(value);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
  RODOS_ASSERT_IFNOT_RETURN_VOID(newPosition <= std::size(buffer));
  std::memcpy(&buffer[*position], &value, sizeof(value));
  *position = newPosition;
}


auto CreateBeacon(int32_t timestamp, int32_t resetCounter, bool eduIsAlive, int32_t gpioBitField)
{
  constexpr auto startByte = '?';
  constexpr auto stopByte = '\n';
  uint8_t checksum = 0;
  // TODO: Think of a better name
  // NOLINTNEXTLINE(readability-identifier-naming)
  auto eduIsAlive_ = static_cast<int32_t>(eduIsAlive);
  constexpr auto beaconSize = sizeof(startByte) + sizeof(timestamp) + sizeof(resetCounter)
                            + sizeof(eduIsAlive_) + sizeof(gpioBitField) + sizeof(checksum)
                            + sizeof(stopByte);

  // TODO: Use std::array of uint8_t or std::byte, or use an etl::vector and create a push_back-like
  // function to fill it
  auto beacon = std::array<uint8_t, beaconSize>{};
  // TODO: Think of a better name; maybe bytes wasn't that bad :shrug:
  auto rawBeacon = std::as_writable_bytes(std::span(beacon));
  size_t i = 0;
  CopyTo(rawBeacon, &i, startByte);
  CopyTo(rawBeacon, &i, timestamp);
  CopyTo(rawBeacon, &i, resetCounter);
  CopyTo(rawBeacon, &i, eduIsAlive_);
  CopyTo(rawBeacon, &i, gpioBitField);
  // TODO: Turn this into function and get rid of warnings
  checksum =
    static_cast<uint8_t>(std::accumulate(std::begin(beacon) + 1, std::end(beacon) - 1, 0) & 0xFF);
  CopyTo(rawBeacon, &i, checksum);
  CopyTo(rawBeacon, &i, stopByte);
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
      auto gpioBitField = CreateGpioBitField(epsIsCharging, epsBatteryIsGood, eduHasUpdate);
      auto eduIsAlive = false;
      // Get the lastet value
      eduIsAliveBuffer.get(eduIsAlive);

      auto beacon =
        CreateBeacon(timestamp, static_cast<int32_t>(resetCounter), eduIsAlive, gpioBitField);
      for(auto c : beacon)
      {
        uart_stdout.putcharNoWait(c);
      }
    }
  }
};


auto const beaconThread = BeaconThread();
}
