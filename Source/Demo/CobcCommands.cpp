#include "CobcCommands.hpp"

#include <rodos.h>

#include <etl/string.h>


namespace rpg
{
// PB0 = GPIO_016 ... EDU on/off
auto eduOnOffPin = HAL_GPIO(GPIO_016);
// When compiling for COBC, use pins GPIO_015 and GPIO_010
// auto uart1 = HAL_UART(UART_IDX1, GPIO_015, GPIO_010);
// When compiling for Nucleo, use pins GPIO_009 and GPIO_010
// auto uart1 = HAL_UART(UART_IDX1, GPIO_009, GPIO_010);

extern HAL_UART uart1;


void TurnEduOn()
{
  PRINTF("*Turn on EDU*\n");
}


void TurnEduOff()
{
  PRINTF("*Turn off EDU*\n");
}


void SendResetCounter(const etl::string<cmdLengthEtl> & command)
{
  PRINTF("*Send reset counter*\n");
}


void SendTemperature(const etl::string<cmdLengthEtl> & command)
{
  PRINTF("*Send temperature*\n");
  uart1.write(command.data(), std::size(command));
  uart1.write("\n", 1);
}


void SendImage(const etl::string<cmdLengthEtl> & command)
{
  PRINTF("*Send image*\n");
  uart1.write(command.data(), std::size(command));
  uart1.write("\n", 1);
}
}