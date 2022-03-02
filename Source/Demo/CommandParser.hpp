#pragma once

#include <rodos.h>


constexpr auto pb0 = GPIO_016;

inline auto eduEnabledGpio = HAL_GPIO(pb0);

// When compiling for COBC, use pins GPIO_015 and GPIO_010
inline auto uart1 = HAL_UART(UART_IDX1, GPIO_015, GPIO_010);
// When compiling for Nucleo, use pins GPIO_009 and GPIO_010
// inline auto uart1 = HAL_UART(UART_IDX1, GPIO_009, GPIO_010);
