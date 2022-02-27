#include "CobcCommands.hpp"

#include <rodos.h>

#include <etl/array.h>
#include <etl/delegate_service.h>
#include <etl/map.h>
#include <etl/string.h>

#include <array>

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming,
// cppcoreguidelines-avoid-non-const-global-variables)

// Include the following line to be able to read from UART when compiling for STM32
extern HAL_UART uart_stdout;
}

namespace rpg
{

// When compiling for COBC, use pins GPIO_015 and GPIO_010
// auto uart1 = HAL_UART(UART_IDX1, GPIO_015, GPIO_010);
// When compiling for Nucleo, use pins GPIO_009 and GPIO_010
auto uart1 = HAL_UART(UART_IDX1, GPIO_009, GPIO_010);

// Give and ID to every command to be called from the delegate service afterwards
enum CommandId
{
  TURN_EDU_ON = 1,
  TURN_EDU_OFF = 2,
  SEND_RESET_COUNTER = 3,
  SEND_TEMPERATURE = 51,
  SEND_IMAGE = 52,
};


// Maps command strings to delegate IDs
auto const cmdStringToId = etl::make_map<etl::string<cmdLengthEtl>, CommandId>(
  etl::pair<etl::string<cmdLengthEtl>, CommandId>{"$01", TURN_EDU_ON},
  etl::pair<etl::string<cmdLengthEtl>, CommandId>{"$02", TURN_EDU_OFF},
  etl::pair<etl::string<cmdLengthEtl>, CommandId>{"$03", SEND_RESET_COUNTER},
  etl::pair<etl::string<cmdLengthEtl>, CommandId>{"$51", SEND_TEMPERATURE},
  etl::pair<etl::string<cmdLengthEtl>, CommandId>{"$52", SEND_IMAGE});


// class ReaderThread : public StaticThread<>
// {
//   void init() override
//   {

//   }

//   void run() override
//   {
//     while(true)
//     {
//       char readChar;
//       uart1.read(&readChar, 1);
//       PRINTF("*EDU Data ready*\n");
//       switch (readChar)
//       {
//       case 'i':
//         // Image incoming
//         break;

//       case '$':
//         // Forward to PC
//         char readBuffer[maxDataLengthEtl + 1];
//         uart1.read(readBuffer, maxDataLengthEtl);
//         PRINTF("%s\n", readBuffer);
//         break;

//       default:
//         break;
//       }
//       uart1.suspendUntilDataReady();
//     }
//   }

// };


class CommandParserThread : public StaticThread<>
{
  void init() override
  {
    auto baudrate = 115'200;
    uart1.init(baudrate);
  }


  int DispatchCommand(const etl::string<cmdLengthEtl> & command,
                      const etl::string<maxDataLengthEtl> & data)
  {
    // Get ID from string
    if(cmdStringToId.count(command) == 0)
    {
      // Error, command not found in map
      PRINTF("*Error, invalid command*\n");
      return -1;
    }

    auto commandId = cmdStringToId.at(command);

    switch(commandId)
    {
      case TURN_EDU_ON:
        TurnEduOn();
        break;

      case TURN_EDU_OFF:
        TurnEduOff();
        break;

      case SEND_RESET_COUNTER:
        SendResetCounter(command);
        break;

      case SEND_TEMPERATURE:
        SendTemperature(command);
        break;

      case SEND_IMAGE:
        SendImage(command);
        break;

      default:
        return -1;
        break;
    }

    return 1;
  }


  void run() override
  {
    etl::string<cmdLengthEtl> currentCommand;
    etl::string<maxDataLengthEtl> currentData;
    bool atCommand = true;
    size_t nDataChars = 0;
    size_t nCommandChars = 0;

    while(true)
    {
      char readChar;
      auto nReceived = uart_stdout.read(&readChar, 1);
      if(nReceived != 0)
      {
        PRINTF("%c", readChar);
        if(readChar == '\r' or readChar == '\n')
        {
          PRINTF("\r\n");
          // Dispatch Command
          atCommand = true;

          currentData.initialize_free_space();
          DispatchCommand(currentCommand, currentData);

          currentCommand.clear();
          currentData.clear();

          nCommandChars = 0;
          nDataChars = 0;
        }
        else if(nCommandChars == cmdLengthEtl and atCommand)
        {
          // Data part begins
          atCommand = false;
        }
        else if(atCommand)
        {
          // Add read char to command
          currentCommand += readChar;
          nCommandChars++;
        }
        else
        {
          // Add read char to data
          currentData += readChar;
          nDataChars++;
        }
      }
      uart_stdout.suspendUntilDataReady();
    }
  }
};

// auto const reader = ReaderThread();
auto const commandParser = CommandParserThread();

}
