#include "CommandParser.hpp"

#include "CobcCommands.hpp"

#include <type_safe/types.hpp>

#include <rodos.h>

#include <etl/map.h>


namespace RODOS
{
// Include the following line to be able to read from UART when compiling for STM32
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}


namespace rpg
{
namespace ts = type_safe;


class ReaderThread : public StaticThread<>
{
    void init() override
    {
        constexpr auto baudrate = 9'600U;
        uart1.init(baudrate);
    }

    void run() override
    {
        using ts::operator""_isize;
        // TODO: There must be a nicer way to do this than reading single characters with random
        // delays

        // Print everything received from EDU UART
        char character = 0;
        while(true)
        {
            while(uart1.read(&character, 1) > 0)
            {
                PRINTF("%c", character);
            }
            constexpr auto delay = 100_isize * MILLISECONDS;
            uart1.suspendUntilDataReady(NOW() + delay.get());
        }
    }
} readerThread;


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
}
