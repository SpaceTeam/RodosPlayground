//! @file
//! @brief  A program for testing the two UARTs of the COBC.
//!
//! If you flash `HelloUart.bin` onto the COBC, the messages "Hello from UART1" and "Hello from
//! UART2" are alternately written every 500 ms to the COBC EDU UART and the COBC UCI UART,
//! respectively. Both UARTs use the same configuration: 115200 baud, 8 data bits, no parity, 1 stop
//! bit.

#include "Communication.hpp"
#include "Io.hpp"

#include <type_safe/narrow_cast.hpp>
#include <type_safe/types.hpp>

#include <rodos.h>

#include <cstring>
#include <string_view>

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}


namespace rpg
{
HAL_GPIO greenLed(ledPin);

HAL_UART huart1(UART_IDX1, GPIO_009, GPIO_010);

namespace ts = type_safe;
using ts::operator""_usize;

auto CreateFakeEduDataFrame(void)  // NOLINT(performance-unnecessary-value-param)
{
    constexpr auto eduDataFrameSize = 1 + 5 * (1 + 4) + 1;
    auto eduDataFrame = std::array<std::byte, eduDataFrameSize>{};
    char startByte = '?';
    char endByte = '\n';
    int32_t a, b, c, d, e;
    a = 256;
    b = -65536;
    c = 0;
    d = 2'147'483'647, e = 0;
    int8_t ida = 1, idb = 2, idc = 3, idd = 4, ide = 5;

    std::memcpy(&eduDataFrame[0], &startByte, sizeof(startByte));

    std::memcpy(&eduDataFrame[1], &ida, 1);
    std::memcpy(&eduDataFrame[2], &a, sizeof(a));

    std::memcpy(&eduDataFrame[6], &idb, 1);
    std::memcpy(&eduDataFrame[7], &b, sizeof(a));

    std::memcpy(&eduDataFrame[11], &idc, 1);
    std::memcpy(&eduDataFrame[12], &c, sizeof(c));

    std::memcpy(&eduDataFrame[16], &idd, 1);
    std::memcpy(&eduDataFrame[17], &d, sizeof(d));

    std::memcpy(&eduDataFrame[21], &ide, 1);
    std::memcpy(&eduDataFrame[22], &e, sizeof(e));

    std::memcpy(&eduDataFrame[26], &endByte, sizeof(endByte));


    return eduDataFrame;
}


class HelloUart : public StaticThread<>
{
    void init() override
    {
        greenLed.init(/*isOutput=*/true, 1, 0);
        constexpr auto baudrate = 9600;
        huart1.init(baudrate);
    }


    void run() override
    {
        using std::operator""sv;

        auto toggle = true;

        TIME_LOOP(0, 1000 * MILLISECONDS)
        {
            greenLed.setPins(toggle);
            if(toggle)
            {
                WriteTo(&uart_stdout, "Hello from uart2\n");
            }
            else
            {
                auto eduDataFrame = CreateFakeEduDataFrame();
                WriteTo(&huart1, std::span(eduDataFrame));
            }
            toggle = not toggle;
        }
    }
};


auto const helloUart = HelloUart();
}
