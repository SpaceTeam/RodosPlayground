#include <rodos.h>

#include <etl/bitset.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string_view>

#include "Topics.h"
#include <stm32f4xx_pwr.h>
#include <stm32f4xx_rtc.h>

// Beacon Structure : see https://wiki.tust.at/books/sts1/page/uart-commands-for-pcb-v1
// 1.   Timestamp
// 2.   Reset Counter
// 3.   Edu Heartbeat
// 4.   GPIO bit field

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming, cppcoreguidelines-avoid-non-const-global-variables)
extern HAL_UART uart_stdout;
}

namespace rpg
{
constexpr auto epsChargingPin = GPIO_046;     // PC14
constexpr auto epsBatteryGoodPin = GPIO_047;  // P15
constexpr auto eduEnabledPin = GPIO_016;      // PB0
constexpr auto eduUpdatePin = GPIO_017;       // PB1

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto epsCharging = HAL_GPIO(epsChargingPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto epsBatteryGood = HAL_GPIO(epsBatteryGoodPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto eduEnabled = HAL_GPIO(eduEnabledPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto eduUpdate = HAL_GPIO(eduUpdatePin);

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static auto buf = CommBuffer<int32_t>();

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-interfaces-global-init)
static auto receiverBuf = Subscriber(eduHeartbeatTopic, buf, "beaconReceiverBuf");

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
      auto const resetCounter = static_cast<int32_t>(RTC_ReadBackupRegister(RTC_BKP_DR0));

      auto const isEpsCharging = static_cast<bool>(epsCharging.readPins());
      auto const isEpsBatteryGood = static_cast<bool>(epsBatteryGood.readPins());
      auto const isEduEnabled = static_cast<bool>(eduEnabled.readPins());
      auto const isEduUpdate = static_cast<bool>(eduUpdate.readPins());

      auto gpioBitField =
        ConstructGpioBitfield(isEpsCharging, isEpsBatteryGood, isEduEnabled, isEduUpdate);

      // Get the lastet value
      buf.get(eduHeartbeatState);
      auto const beacon = ConstructBeacon(timestamp, resetCounter, eduHeartbeatState, gpioBitField);
      uart_stdout.write(beacon.data(), beacon.size());
    }
  }
};

auto const beacon = Beacon();
}
