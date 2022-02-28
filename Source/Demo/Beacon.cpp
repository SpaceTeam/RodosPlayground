#include "Topics.hpp"

#include <rodos.h>

#include <etl/bitset.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <string_view>

#include <stm32f4xx_pwr.h>
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
constexpr auto epsChargingPin = GPIO_046;     // PC14
constexpr auto epsBatteryGoodPin = GPIO_047;  // PC15
constexpr auto eduEnabledPin = GPIO_016;      // PB0
constexpr auto eduUpdatePin = GPIO_017;       // PB1

auto epsCharging = HAL_GPIO(epsChargingPin);
auto epsBatteryGood = HAL_GPIO(epsBatteryGoodPin);
auto eduEnabled = HAL_GPIO(eduEnabledPin);
auto eduUpdate = HAL_GPIO(eduUpdatePin);

auto buf = CommBuffer<int32_t>();
auto receiverBuf = Subscriber(eduHeartbeatTopic, buf, "beaconReceiverBuf");


class Beacon : public StaticThread<>
{
  void init() override
  {
    // 1:  Enable the power controller APB1 interface
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    // 2: Enable access to RTC domain using the PWR_BackupAccessCmd() function.
    PWR_BackupAccessCmd(ENABLE);

    epsCharging.init(/*isOutput=*/false, 1, 0);
    epsBatteryGood.init(/*isOutput=*/false, 1, 0);
    eduEnabled.init(/*isOutput=*/true, 1, 0);
    eduUpdate.init(/*isOutput=*/false, 1, 0);
  }

  static auto ConstructGpioBitfield(bool isEpsCharging,
                                    bool isEpsBatteryGood,
                                    bool isEduEnabled,
                                    bool isEduUpdate)
  {
    constexpr auto bitsetSize = sizeof(int32_t) * 8;
    auto gpioBitFieldBitSet = etl::bitset<bitsetSize>();
    gpioBitFieldBitSet.set(0, isEpsCharging);
    gpioBitFieldBitSet.set(1, isEpsBatteryGood);
    gpioBitFieldBitSet.set(2, isEduEnabled);
    gpioBitFieldBitSet.set(3, isEduUpdate);
    return static_cast<int32_t>(gpioBitFieldBitSet.to_ulong());
  }

  static auto ConstructBeacon(int32_t timestamp,
                              int32_t resetCounter,
                              int32_t eduHeartbeatvalue,
                              int32_t gpioBitField)
  {
    auto constexpr startByte = '?';

    // final +1 is for checksum
    auto const beaconSize = sizeof(startByte) + sizeof(eduHeartbeatvalue) + sizeof(resetCounter)
                          + sizeof(gpioBitField) + sizeof(timestamp) + 1;

    auto beacon = etl::string<beaconSize + 1>();
    beacon.initialize_free_space();

    size_t i = 0;
    beacon[i] = startByte;
    i += sizeof(startByte);
    std::memcpy(&beacon[i], &timestamp, sizeof(timestamp));
    i += sizeof(timestamp);
    std::memcpy(&beacon[i], &resetCounter, sizeof(resetCounter));
    i += sizeof(resetCounter);
    std::memcpy(&beacon[i], &eduHeartbeatvalue, sizeof(eduHeartbeatvalue));
    i += sizeof(eduHeartbeatvalue);
    std::memcpy(&beacon[i], &gpioBitField, sizeof(gpioBitField));
    i += sizeof(gpioBitField);

    beacon.uninitialized_resize(beaconSize);

    auto checksum =
      static_cast<uint8_t>(std::accumulate(std::begin(beacon) + 1, std::end(beacon) - 1, 0) & 0xFF);
    std::memcpy(&beacon[i], &checksum, sizeof(checksum));
    return beacon;
  }


  void run() override
  {
    auto backupRegisterValue = RTC_ReadBackupRegister(RTC_BKP_DR0);
    ++backupRegisterValue;
    RTC_WriteBackupRegister(RTC_BKP_DR0, backupRegisterValue);
    auto eduHeartbeatState = static_cast<int32_t>(0);


    TIME_LOOP(0, 2000 * MILLISECONDS)
    {
      auto const timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);
      auto const resetCounter = static_cast<int32_t>(RTC_ReadBackupRegister(RTC_BKP_DR0));

      auto const isEpsCharging = static_cast<bool>(epsCharging.readPins());
      auto const isEpsBatteryGood = static_cast<bool>(epsBatteryGood.readPins());
      // auto const isEduEnabled = static_cast<bool>(eduEnabled.readPins());
      auto const isEduEnabled = false;
      auto const isEduUpdate = static_cast<bool>(eduUpdate.readPins());

      auto gpioBitField =
        ConstructGpioBitfield(isEpsCharging, isEpsBatteryGood, isEduEnabled, isEduUpdate);

      // Get the lastet value
      buf.get(eduHeartbeatState);

      auto beacon = ConstructBeacon(timestamp, resetCounter, eduHeartbeatState, gpioBitField);

      for(auto c : beacon)
      {
        uart_stdout.putcharNoWait(c);
      }
    }
  }
};

auto const beacon = Beacon();
}
