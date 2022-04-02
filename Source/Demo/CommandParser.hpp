#pragma once

#include "Io.hpp"

#include <rodos.h>

namespace rpg
{
inline auto eduEnabledGpio = HAL_GPIO(eduEnabledPin);

inline auto uart1 = HAL_UART(eduUartIndex, eduUartTxPin, eduUartRxPin);
}
