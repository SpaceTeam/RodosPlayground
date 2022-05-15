#include "CommandParser.hpp"

#include "CobcCommands.hpp"
#include "Communication.hpp"
#include "Topics.hpp"

#include <type_safe/index.hpp>
#include <type_safe/types.hpp>

#include <rodos.h>

#include <etl/map.h>
#include <etl/string.h>
#include <etl/string_view.h>

#include <cstring>


namespace RODOS
{
// Include the following line to be able to read from UART when compiling for STM32
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}


namespace rpg
{
namespace ts = type_safe;

using ts::operator""_u8;
using ts::operator""_i32;
using ts::operator""_usize;

auto DispatchCommand(const etl::string<commandSize.get()> & command)
{
    auto targetIsCobc = command[1] == '0';
    auto targetIsEdu = command[1] == '5';
    auto commandId = command[2];

    if(targetIsCobc)
    {
        switch(commandId)
        {
            case '1':
            {
                TurnEduOn();
                return;
            }
            case '2':
            {
                TurnEduOff();
                return;
            }
            case '3':
            {
                SendResetCounter();
                return;
            }
            default:
            {
                break;
            }
        }
    }
    else if(targetIsEdu)
    {
        switch(commandId)
        {
            case '1':
            {
                SendTemperature(command);
                return;
            }
            case '2':
            {
                SendImage(command);
                return;
            }
            default:
            {
                break;
            }
        }
    }

    PRINTF("*Error, invalid command*\n");
}


class CommandParserThread : public StaticThread<>
{
    void init() override
    {
        eduEnabledGpio.init(/*isOutput=*/true, 1, 0);
    }

    void run() override
    {
        constexpr auto startCharacter = '$';

        auto command = etl::string<commandSize.get()>();
        ts::bool_t startWasDetected = false;
        while(true)
        {
            char readCharacter = 0;
            auto nReadCharacters = ts::size_t(uart_stdout.read(&readCharacter, 1));
            if(nReadCharacters != 0U)
            {
                if(readCharacter == startCharacter)
                {
                    startWasDetected = true;
                    command.clear();
                    command += startCharacter;
                }
                else if(startWasDetected)
                {
                    command += readCharacter;
                    if(command.full())
                    {
                        DispatchCommand(command);
                        startWasDetected = false;
                    }
                }
            }
            uart_stdout.suspendUntilDataReady();
        }
    }
} commandParserThread;

// startByte + 1 Id byte + 4 data bytes + endByte
constexpr auto dataFrameSize = 1_usize + 1_usize + 4_usize + 1_usize;

auto EduDataParse(const etl::string<dataFrameSize.get()> & dataFrame)
{
    ts::index_t index = 1_usize;

    auto id = 0_u8;
    auto data = 0_i32;

    std::memcpy(&id, &(at(dataFrame, index)), sizeof(id));
    std::memcpy(&data, &(at(dataFrame, index + 1)), sizeof(data));

    // TODO fix magic constants
    switch(static_cast<ts::integer<unsigned char>::integer_type>(id))
    {
        case 1:  // NOLINT
            temperatureTopic.publish(data);
            break;
        case 2:  // NOLINT
            accelerationXTopic.publish(data);
            break;
        case 3:  // NOLINT
            accelerationYTopic.publish(data);
            break;
        case 4:  // NOLINT
            accelerationZTopic.publish(data);
            break;
        case 5:  // NOLINT
            brightnessTopic.publish(data);
            break;
        default:;
            // Too bad
    }
}

class EduReaderThread : public StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 9'600;
        eduUart.init(baudrate);
    }

    void run() override
    {
        constexpr auto startCharacter = '?';

        // StartByte + 5 channels * (ID byte + data) + stopByte
        auto eduDataFrame = etl::string<dataFrameSize.get()>();
        ts::bool_t startWasDetected = false;
        while(true)
        {
            WriteTo(&eduUart, "$52\n");
            eduUart.suspendUntilWriteFinished();

            char readCharacter = 0;
            auto nReadCharacters = ts::size_t(eduUart.read(&readCharacter, 1));
            if(nReadCharacters != 0U)
            {
                // PRINTF("%c", readCharacter);
                if(readCharacter == startCharacter)
                {
                    // PRINTF("Start Detected\n");
                    startWasDetected = true;
                    eduDataFrame.clear();
                    eduDataFrame += startCharacter;
                }
                else if(startWasDetected)
                {
                    eduDataFrame += readCharacter;
                    if(eduDataFrame.full())
                    {
                        // Command full
                        // TODO maybe check that endbyte is correct (and more checks in general)
                        EduDataParse(eduDataFrame);
                        startWasDetected = false;
                    }
                }
            }
            eduUart.suspendUntilDataReady();
        }
    }
} eduReaderThread;


}
