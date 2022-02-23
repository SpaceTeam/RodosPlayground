#include <rodos.h>

#include <etl/bitset.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string_view>

#include "topics.h"
// Beacon Structure
// 1.   Timestamp
// 2.   Reset Counter
// 3.   Edu Heartbeat
// 4.   GPIO bit field

#include <stm32f4xx_pwr.h>
#include <stm32f4xx_rtc.h>

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming, cppcoreguidelines-avoid-non-const-global-variables)
extern HAL_UART uart_stdout;
}

namespace rpg
{
auto const epsChargingPin = GPIO_046;     // PC14
auto const epsBatteryGoodPin = GPIO_047;  // P15
auto const eduEnabledPin = GPIO_016;      // PB0
auto const eduUpdatePin = GPIO_017;       // PB1

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO epsCharging(epsChargingPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO epsBatteryGood(epsBatteryGoodPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO eduEnabled(eduEnabledPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO eduUpdate(eduUpdatePin);

// NOLINTNEXTLINE
static CommBuffer<int32_t> buf;

// NOLINTNEXTLINE
static Subscriber receiverBuf(eduHeartbeatTopic, buf, "receiverBuf");

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
    eduEnabled.init(/*isOutput=*/false, 1, 0);
    eduUpdate.init(/*isOutput=*/false, 1, 0);
  }


  static auto ConstructGpioBitfield(bool epsChargingValue,
                                    bool epsBatteryGoodValue,
                                    bool eduEnabledValue,
                                    bool eduUpdateValue)
  {
    auto const bitsetSize = sizeof(int32_t) * 8;
    etl::bitset<bitsetSize> gpioBitFieldBitSet;
    gpioBitFieldBitSet.set(0, epsChargingValue);
    gpioBitFieldBitSet.set(1, epsBatteryGoodValue);
    gpioBitFieldBitSet.set(2, eduEnabledValue);
    gpioBitFieldBitSet.set(3, eduUpdateValue);
    return static_cast<int32_t>(gpioBitFieldBitSet.to_ulong());
  }

  static auto ConstructBeacon(int32_t timestamp,
                              int32_t resetCounter,
                              int32_t eduHeartbeatvalue,
                              int32_t gpioBitField)
  {
    auto const beaconSize = sizeof(eduHeartbeatvalue) + sizeof(resetCounter) + sizeof(gpioBitField)
                          + sizeof(timestamp) + sizeof('?');
    std::array<std::byte, beaconSize> beacon{};

    size_t i = 0;

    // NOLINTNEXTLINE
    beacon[i] = static_cast<std::byte>('?');
    i += sizeof('?');
    // NOLINTNEXTLINE
    std::memcpy(&beacon[i], &timestamp, sizeof(timestamp));
    i += sizeof(timestamp);
    // NOLINTNEXTLINE
    std::memcpy(&beacon[i], &resetCounter, sizeof(resetCounter));
    i += sizeof(resetCounter);
    // NOLINTNEXTLINE
    std::memcpy(&beacon[i], &eduHeartbeatvalue, sizeof(eduHeartbeatvalue));
    i += sizeof(eduHeartbeatvalue);
    // NOLINTNEXTLINE
    std::memcpy(&beacon[i], &gpioBitField, sizeof(gpioBitField));
    // NOLINTNEXTLINE
    i += sizeof(gpioBitField);

    return beacon;
  }

  void run() override
  {
    auto backupRegisterValue = RTC_ReadBackupRegister(RTC_BKP_DR0);
    ++backupRegisterValue;
    RTC_WriteBackupRegister(RTC_BKP_DR0, backupRegisterValue);


    auto eduHeartbeatState = static_cast<int32_t>(0);
    TIME_LOOP(0, 5000 * MILLISECONDS)
    {
      auto const timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);

      // NOLINTNEXTLINE
      auto resetCounter = static_cast<int32_t>(RTC_ReadBackupRegister(RTC_BKP_DR0));

      auto epsChargingValue = static_cast<bool>(epsCharging.readPins());
      auto epsBatteryGoodValue = static_cast<bool>(epsBatteryGood.readPins());
      auto eduEnabledValue = static_cast<bool>(eduEnabled.readPins());
      auto eduUpdateValue = static_cast<bool>(eduUpdate.readPins());

      auto gpioBitField = ConstructGpioBitfield(
        /*epsChargingValue=*/epsChargingValue,
        /*epsBatteryGoodValue=*/epsBatteryGoodValue,
        /*eduEnabledValue=*/eduEnabledValue,
        /*eduUpdateValue=*/eduUpdateValue);

      // Get the lastet value
      buf.get(eduHeartbeatState);
      auto const beacon = ConstructBeacon(timestamp, resetCounter, eduHeartbeatState, gpioBitField);
      uart_stdout.write(beacon.data(), beacon.size());
    }
  }
};

auto const beacon = Beacon();
}
