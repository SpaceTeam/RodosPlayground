#include "CobcCommands.hpp"

#include "CommandParser.hpp"

#include <Io.hpp>

#include <stm32f4xx_rtc.h>


namespace rpg
{
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


void SendResetCounter()
{
  PRINTF("*Send reset counter*\n");
  auto resetCount = RTC_ReadBackupRegister(RTC_BKP_DR0);
  PRINTF("$Reset counter = %5i\n", resetCount);
}


void SendTemperature(etl::string<commandSize> const & command)
{
  PRINTF("*Send temperature*\n");
  uart1.write(std::data(command), std::size(command));
  uart1.write("\n", 1);
}


void SendImage(etl::string<commandSize> const & command)
{
  PRINTF("*Send image*\n");
  uart1.write(std::data(command), std::size(command));
  uart1.write("\n", 1);
}
}
