#pragma once

#include <type_safe/types.hpp>

#include <etl/string.h>


namespace rpg
{
using type_safe::operator""_usize;


constexpr auto commandSize = 3_usize;


void TurnEduOn();
void TurnEduOff();
}
