#include "CobcCommands.hpp"

#include "CommandParser.hpp"

#include <rodos.h>


namespace rpg
{
void TurnEduOn()
{
  PRINTF("*Turn on EDU*\n");
  // FIXME: Geht nicht! Weil nirgendwo die init() Funktion aufgerufen wird! Gehört am ehesten ins
  // CommandParser.cpp
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