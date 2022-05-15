#include "CobcCommands.hpp"

#include "CommandParser.hpp"
#include "Io.hpp"

#include <rodos.h>

#include <stm32f4xx_rtc.h>


namespace rpg
{
namespace ts = type_safe;


void TurnEduOn()
{
    PRINTF("*Turn on EDU*\n");
    // The enable pin uses inverted logic: 0 = enable
    eduEnabledGpio.setPins(0U);
}


void TurnEduOff()
{
    PRINTF("*Turn off EDU*\n");
    // The enable pin uses inverted logic: 1 = disable
    eduEnabledGpio.setPins(1U);
}


void SendResetCounter()
{
    PRINTF("*Send reset counter*\n");
    auto resetCount = ts::size_t(RTC_ReadBackupRegister(RTC_BKP_DR0));
    PRINTF("$Reset counter = %5lu\n", static_cast<unsigned long>(resetCount.get()));
}


void SendTemperature(etl::string<commandSize.get()> const & command)
{
    PRINTF("*Send temperature*\n");
    eduUart.write(std::data(command), std::size(command));
    eduUart.write("\n", 1U);
}


void SendImage(etl::string<commandSize.get()> const & command)
{
    PRINTF("*Send image*\n");
    eduUart.write(std::data(command), std::size(command));
    eduUart.write("\n", 1U);
}
}
