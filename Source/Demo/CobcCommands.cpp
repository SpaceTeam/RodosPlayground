#include <rodos.h>
#include "CobcCommands.hpp"

void turnEduOn(const size_t id)
{
  PRINTF("*Turn on EDU*\n");
}

void turnEduOff(const size_t id)
{
  PRINTF("*Turn off EDU*\n");
}

void sendResetCounter(const size_t id)
{
  PRINTF("*Send reset counter*\n");
}

void sendTemperature(const size_t id)
{
  PRINTF("*Send temperature*\n");
}

void sendImage(const size_t id)
{
  PRINTF("*Send image*\n");
}

void unhandledCommand(const size_t id)
{
  PRINTF("*Invalid command*\n");
}