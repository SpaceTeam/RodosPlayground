#include "Communication.hpp"
#include "Io.hpp"
#include "Topics.hpp"

#include <type_safe/narrow_cast.hpp>
#include <type_safe/types.hpp>

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
namespace ts = type_safe;
using ts::operator""_u8;
using ts::operator""_i32;
using ts::operator""_isize;
using ts::operator""_usize;

constexpr auto nBits = 32;
using BitField = etl::bitset<nBits>;


auto eduUpdateGpio = HAL_GPIO(eduUpdatePin);
auto epsChargingGpio = HAL_GPIO(epsChargingPin);
auto epsBatteryGoodGpio = HAL_GPIO(epsBatteryGoodPin);

auto eduIsAliveBuffer = CommBuffer<bool>();
auto eduIsAliveSubscriber = Subscriber(eduIsAliveTopic, eduIsAliveBuffer, "eduIsAliveSubscriber");


auto CreateGpioBitField(ts::bool_t epsIsCharging,
                        ts::bool_t epsBatteryIsGood,
                        ts::bool_t eduHasUpdate)
{
    auto gpioBitField = BitField();
    gpioBitField.set(0, bool(epsIsCharging));
    gpioBitField.set(1, bool(epsBatteryIsGood));
    gpioBitField.set(2, bool(eduHasUpdate));
    return gpioBitField;
}


auto CopyTo(std::span<std::byte> buffer, ts::size_t * const position, auto value)
{
    auto newPosition = *position + sizeof(value);
    RODOS_ASSERT_IFNOT_RETURN_VOID(newPosition <= std::size(buffer));
    std::memcpy(&buffer[(*position).get()], &value, sizeof(value));
    *position = newPosition;
}


auto ComputeChecksum(std::span<std::byte> beacon)
{
    return std::accumulate(std::begin(beacon) + 1,
                           std::end(beacon) - 2,
                           0_u8,
                           [](auto sum, auto currentElement)
                           {
                               ts::uint8_t x = static_cast<uint8_t>(currentElement);
                               return sum + x;
                           });
}


auto CreateBeacon(ts::int64_t timestamp,
                  ts::uint32_t resetCounter,
                  ts::bool_t eduIsAlive,
                  BitField gpioBitField)  // NOLINT(performance-unnecessary-value-param)
{
    constexpr auto startByte = '?';
    constexpr auto stopByte = '\n';
    // TODO: Think of better names
    auto beaconTimestamp = ts::narrow_cast<ts::int32_t>(timestamp);
    auto beaconResetCounter = ts::narrow_cast<ts::int32_t>(resetCounter);
    auto beaconGpioBitField = ts::narrow_cast<ts::int32_t>(gpioBitField.to_ulong());
    auto beaconEduIsAlive = eduIsAlive ? 1_i32 : 0_i32;
    auto checksum = 0_u8;
    constexpr auto beaconSize = sizeof(startByte) + sizeof(beaconTimestamp)
                              + sizeof(beaconResetCounter) + sizeof(beaconEduIsAlive)
                              + sizeof(beaconGpioBitField) + sizeof(checksum) + sizeof(stopByte);

    // TODO: Use std::array of uint8_t or std::byte, or use an etl::vector and create a
    // push_back-like function to fill it
    auto beacon = std::array<std::byte, beaconSize>{};
    auto position = 0_usize;
    CopyTo(beacon, &position, startByte);
    CopyTo(beacon, &position, beaconTimestamp);
    CopyTo(beacon, &position, beaconResetCounter);
    CopyTo(beacon, &position, beaconEduIsAlive);
    CopyTo(beacon, &position, beaconGpioBitField);
    checksum = ComputeChecksum(beacon);
    CopyTo(beacon, &position, checksum);
    CopyTo(beacon, &position, stopByte);
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

        epsChargingGpio.init(/*isOutput=*/false, 1U, 0U);
        epsBatteryGoodGpio.init(/*isOutput=*/false, 1U, 0U);
        eduUpdateGpio.init(/*isOutput=*/false, 1U, 0U);
    }

    void run() override
    {
        // TODO: Move to init()
        ts::uint32_t resetCounter = RTC_ReadBackupRegister(RTC_BKP_DR0);
        ++resetCounter;
        RTC_WriteBackupRegister(RTC_BKP_DR0, resetCounter.get());

        TIME_LOOP(0, (2'000_isize * MILLISECONDS).get())
        {
            ts::int64_t const timestamp = NOW() / MILLISECONDS;
            ts::bool_t const epsIsCharging = epsChargingGpio.readPins() != 0;
            ts::bool_t const epsBatteryIsGood = epsBatteryGoodGpio.readPins() != 0;
            ts::bool_t const eduHasUpdate = eduUpdateGpio.readPins() != 0;
            auto gpioBitField = CreateGpioBitField(epsIsCharging, epsBatteryIsGood, eduHasUpdate);
            auto eduIsAlive = false;
            // Get the lastet value
            eduIsAliveBuffer.get(eduIsAlive);
            auto beacon = CreateBeacon(timestamp, resetCounter, eduIsAlive, gpioBitField);

            WriteTo(&uart_stdout, std::span(beacon));
        }
    }
};


auto const beaconThread = BeaconThread();
}
