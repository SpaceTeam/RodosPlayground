//! @file
//! @brief  A program for testing the four SPIs of the COBC.
//!
//! Prerequisites:
//!   - requires UART2 = COBC UCI UART to work
//!
//! Preparation:
//!   - short MISO and MOSI lines of SPI1 to SPI4
//!
//! If you flash `HelloSpi.bin` onto the COBC, the messages "Hello from SPI#!" with # = 1–4, are
//! written every 2 s to the MOSI line of the corresponding SPI. The matching MISO line is read at
//! the same time. The thusly received message is printed to the COBC UCI UART. If, e.g., the round
//! trip of the SPI2 message was successful, the UART output looks something like the following.
//!
//! ```
//! Writing to SPI2
//! nReceivedBytes = 16
//! answer = 'Hello from SPI2!'
//! ```

#include "Communication.hpp"
#include "Io.hpp"

#include <type_safe/types.hpp>

#include <rodos.h>

#include <etl/string.h>

#include <string_view>


namespace rpg
{
auto spis = std::array{HAL_SPI(flashSpiIndex, flashSpiSckPin, flashSpiMisoPin, flashSpiMosiPin),
                       HAL_SPI(framSpiIndex, framSpiMisoPin, framSpiMisoPin, framSpiMosiPin),
                       HAL_SPI(cobcSpiIndex, cobcSpiSckPin, cobcSpiMisoPin, cobcSpiMosiPin),
                       HAL_SPI(rfSpiIndex, rfSpiSckPin, rfSpiMisoPin, rfSpiMosiPin)};


class HelloSpi : public StaticThread<>
{
    void init() override
    {
        for(auto & spi : spis)
        {
            spi.init();
        }
    }

    void run() override
    {
        namespace ts = type_safe;
        using std::operator""sv;
        using ts::operator""_usize;

        constexpr auto messages = std::array{
            "Hello from SPI1!"sv, "Hello from SPI2!"sv, "Hello from SPI3!"sv, "Hello from SPI4!"sv};
        static_assert(std::size(spis) == std::size(messages));

        auto i = 0_usize;
        TIME_LOOP(0, 500 * MILLISECONDS)
        {
            // PRINTF is super weird because without the static_cast %u causes "expected unsigned
            // int but got unsigned long" and %lu causes "expected unsigned long but got unsigned
            // int". I guess this is because int and long are both 32bit on an STM32.
            PRINTF("Writing to SPI%lu\n", static_cast<unsigned long>((i + 1U).get()));
            auto answer = etl::string<std::size(messages[0])>();
            ts::int_t nReceivedBytes = WriteToReadFrom(&spis[i.get()], messages[i.get()], &answer);
            PRINTF("nReceivedBytes = %i\n", nReceivedBytes.get());
            PRINTF("answer = '%s'\n\n", answer.c_str());

            i++;
            i %= std::size(spis);
        }
    }
};


auto const helloSpi = HelloSpi();
}
