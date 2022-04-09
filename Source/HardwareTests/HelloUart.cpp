//! @file
//! @brief  A program for testing the two UARTs of the COBC.
//!
//! If you flash `HelloUart.bin` onto the COBC, the messages "Hello from UART1" and "Hello from
//! UART2" are alternately written every 500 ms to the COBC EDU UART and the COBC UCI UART,
//! respectively. Both UARTs use the same configuration: 115200 baud, 8 data bits, no parity, 1 stop
//! bit.

#include "Io.hpp"
#include "Write.hpp"

#include <rodos.h>

#include <string_view>

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}

namespace rpg
{
auto eduUart = HAL_UART(eduUartIndex, eduUartTxPin, eduUartRxPin);

class HelloUart : public StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 115'200;
        eduUart.init(baudrate);
    }

    void run() override
    {
        using std::operator""sv;

        auto toggle = true;

        TIME_LOOP(0, 500 * MILLISECONDS)
        {
            if(toggle)
            {
                auto myText = "Hello from UART2 - with templated function\r\n"sv;
                WriteTo(uart_stdout, myText);
            }
            else
            {
                // NOLINTNEXTLINE
                std::array<uint16_t, 5> data = {1, 2, 3, 4, 5};
                auto span = std::span{data};
                WriteTo(uart_stdout, span);
            }
            toggle = not toggle;
        }
    }
};


auto const helloUart = HelloUart();
}
