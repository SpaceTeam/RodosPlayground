#include "Communication.hpp"
#include "Io.hpp"
#include "Utility.hpp"

#include <type_safe/types.hpp>

#include <rodos_no_using_namespace.h>

#include <array>
#include <cstring>


namespace rpg
{
RODOS::HAL_UART uart1(RODOS::UART_IDX1, eduUartTxPin, eduUartRxPin);

namespace ts = type_safe;
using ts::operator""_usize;

auto CreateFakeEduDataFrame()
{
    char startByte = '?';
    constexpr int8_t id = 3;
    constexpr int32_t data = 123459876;
    char endByte = '\n';

    constexpr auto eduDataFrameSize =
        sizeof(startByte) + sizeof(id) + sizeof(data) + sizeof(endByte);
    auto eduDataFrame = std::array<std::byte, eduDataFrameSize>{};
    ts::size_t index = 0_usize;

    util::CopyTo(eduDataFrame, &index, startByte);
    util::CopyTo(eduDataFrame, &index, id);
    util::CopyTo(eduDataFrame, &index, data);
    util::CopyTo(eduDataFrame, &index, endByte);

    return eduDataFrame;
}


class EduDataFrameFaker : public RODOS::StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 9'600;
        uart1.init(baudrate);
    }


    void run() override
    {
        TIME_LOOP(0, 1000 * RODOS::MILLISECONDS)
        {
            auto eduDataFrame = CreateFakeEduDataFrame();
            WriteTo(&uart1, std::span(eduDataFrame));
        }
    }
};


auto const eduDataFrameFaker = EduDataFrameFaker();
}
