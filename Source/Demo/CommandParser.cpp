#include "CommandParser.hpp"

#include "CobcCommands.hpp"

#include <etl/map.h>


namespace RODOS
{
// Include the following line to be able to read from UART when compiling for STM32
// NOLINTNEXTLINE(readability-identifier-naming)
extern HAL_UART uart_stdout;
}

namespace rpg
{
class ReaderThread : public StaticThread<>
{
  void init() override
  {
    constexpr auto baudrate = 9'600U;
    uart1.init(baudrate);
  }

  void run() override
  {
    // TODO: There must be a nicer way to do this than reading single characters with random delays

    // Print everything received from EDU UART
    char character = 0;
    while(true)
    {
      while(uart1.read(&character, 1) > 0)
      {
        PRINTF("%c", character);
      }
      constexpr auto delay = 100 * MILLISECONDS;
      uart1.suspendUntilDataReady(NOW() + delay);
    }
  }
} readerThread;


enum CommandId
{
  turnEduOn = 1,
  turnEduOff = 2,
  sendResetCounter = 3,
  sendTemperature = 51,
  sendImage = 52,
};


auto const commandStringToId = etl::make_map<etl::string<commandSize>, CommandId>(
  etl::pair<etl::string<commandSize>, CommandId>{"$01", turnEduOn},
  etl::pair<etl::string<commandSize>, CommandId>{"$02", turnEduOff},
  etl::pair<etl::string<commandSize>, CommandId>{"$03", sendResetCounter},
  etl::pair<etl::string<commandSize>, CommandId>{"$51", sendTemperature},
  etl::pair<etl::string<commandSize>, CommandId>{"$52", sendImage});


auto DispatchCommand(const etl::string<commandSize> & command)
{
  if(not commandStringToId.contains(command))
  {
    PRINTF("*Error, invalid command*\n");
    return;
  }

  auto commandId = commandStringToId.at(command);
  switch(commandId)
  {
    case turnEduOn:
    {
      TurnEduOn();
      break;
    }
    case turnEduOff:
    {
      TurnEduOff();
      break;
    }
    case sendResetCounter:
    {
      SendResetCounter();
      break;
    }
    case sendTemperature:
    {
      SendTemperature(command);
      break;
    }
    case sendImage:
    {
      SendImage(command);
      break;
    }
  }
}


class CommandParserThread : public StaticThread<>
{
  void init() override
  {
    eduEnabledGpio.init(/*isOutput=*/true, 1, 0);
  }

  void run() override
  {
    constexpr auto startCharacter = '$';

    auto command = etl::string<commandSize>();
    bool startWasDetected = false;
    while(true)
    {
      char readCharacter = 0;
      auto nReadCharacters = uart_stdout.read(&readCharacter, 1);
      if(nReadCharacters != 0)
      {
        if(readCharacter == startCharacter)
        {
          startWasDetected = true;
          command.clear();
          command += startCharacter;
        }
        else if(startWasDetected)
        {
          command += readCharacter;
          if(command.full())
          {
            DispatchCommand(command);
            startWasDetected = false;
          }
        }
      }
      uart_stdout.suspendUntilDataReady();
    }
  }
} commandParserThread;
}
