#include "CobcCommands.hpp"

#include <rodos.h>


namespace rpg
{
constexpr auto pb0 = GPIO_016;  // PB0


extern HAL_UART uart1;
auto eduEnabledGpio = HAL_GPIO(pb0);


void TurnEduOn()
{
  PRINTF("*Turn on EDU*\n");
  eduEnabledGpio.setPins(1);
}


void TurnEduOff()
{
  PRINTF("*Turn off EDU*\n");
  eduEnabledGpio.setPins(0);
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