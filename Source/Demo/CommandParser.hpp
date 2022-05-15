#pragma once

#include "Io.hpp"

#include <rodos.h>


namespace rpg
{
// TODO: Why is this here? Wouldn't it fit better in CobcCommands.hpp?
inline auto eduEnabledGpio = HAL_GPIO(eduEnabledPin);

inline auto eduUart = HAL_UART(eduUartIndex, eduUartTxPin, eduUartRxPin);
}
