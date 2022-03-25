#pragma once

#include "Pins.hpp"

namespace rpg
{
inline constexpr auto ledPin = pa13;

inline constexpr auto eduUartIndex = RODOS::UART_IDX1;
inline constexpr auto uciUartIndex = RODOS::UART_IDX2;

inline constexpr auto eduUartTxPin = pa15;
inline constexpr auto eduUartRxPin = pa10;
inline constexpr auto uciUartTxPin = pa2;
inline constexpr auto uciUartRxPin = pa3;

inline constexpr auto eduUpdatePin = pb1;
inline constexpr auto epsChargingPin = pc14;
inline constexpr auto epsBatteryGoodPin = pc15;
inline constexpr auto eduHeartbeatPin = pc5;
inline constexpr auto eduEnabledPin = pb0;

// SPI1 = Flash SPI
inline constexpr auto spi1Sck = pa5;
inline constexpr auto spi1Miso = pa6;
inline constexpr auto spi1Mosi = pa7;
// SPI2 = FRAM SPI
inline constexpr auto spi2Sck = pc7;
inline constexpr auto spi2Miso = pc2;
inline constexpr auto spi2Mosi = pc3;
// SPI3 = COBC SPI
inline constexpr auto spi3Sck = pc10;
inline constexpr auto spi3Miso = pc11;
inline constexpr auto spi3Mosi = pc12;
// SPI4 = RF SPI
inline constexpr auto spi4Sck = pb13;
inline constexpr auto spi4Miso = pa11;
inline constexpr auto spi4Mosi = pa1;

}
