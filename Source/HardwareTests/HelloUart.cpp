//! @file
//! @brief  A program for testing the two UARTs of the COBC.
//!
//! If you flash `HelloUart.bin` onto the COBC, the messages "Hello from UART1" and "Hello from
//! UART2" are alternately written every 500 ms to the COBC EDU UART and the COBC UCI UART,
//! respectively. Both UARTs use the same configuration: 115200 baud, 8 data bits, no parity, 1 stop
//! bit.

#include "Communication.hpp"
#include "Io.hpp"

#include <rodos.h>

#include <string_view>

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}


namespace rpg
{
HAL_GPIO greenLed(ledPin);

HAL_UART uart1(UART_IDX1, GPIO_009, GPIO_010);

class HelloUart : public StaticThread<>
{
    void init() override
    {
        greenLed.init(/*isOutput=*/true, 1, 0);
        constexpr auto baudrate = 9'600;
        uart1.init(baudrate);
    }


    void run() override
    {
        using std::operator""sv;

        auto toggle = true;

        TIME_LOOP(0, 1000 * MILLISECONDS)
        {
            greenLed.setPins(static_cast<uint32_t>(toggle));
            if(toggle)
            {
                WriteTo(&uart_stdout, "Hello from uart2\n");
            }
            else
            {
                WriteTo(&uart_stdout, "Hello from uart1\n");
            }
            toggle = not toggle;
        }
    }
};


auto const helloUart = HelloUart();
}
