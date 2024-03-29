#include "CobcCommands.hpp"

#include "CommandParser.hpp"
#include "Io.hpp"

#include <rodos.h>

#include <stm32f4xx_rtc.h>


namespace rpg
{
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
}
