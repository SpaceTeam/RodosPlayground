#pragma once

#include <etl/string.h>


namespace rpg
{
constexpr auto commandSize = 3;


void TurnEduOn();
void TurnEduOff();
void SendResetCounter();
void SendTemperature(etl::string<commandSize> const & command);
void SendImage(etl::string<commandSize> const & command);
}
