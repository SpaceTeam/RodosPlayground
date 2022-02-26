#include <rodos.h>
#include <array>
#include <etl/map.h>
#include <etl/string.h>
#include <etl/delegate_service.h>
#include "CobcCommands.hpp"

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming)

// Include the following line to be able to read from UART when compiling for STM32
// extern HAL_UART uart_stdout;
}

namespace rpg 
{

// ETL Strings with internal buffer -> no null termination needed
constexpr auto CMD_LENGTH_ETL = 3;

// Give and ID to every command to be called from the delegate service afterwards
enum CommandId
{
  TURN_EDU_ON = 1,
  TURN_EDU_OFF = 2,
  SEND_RESET_COUNTER = 3,
  SEND_TEMPERATURE = 51,
  SEND_IMAGE = 52,
  ID_END,
  ID_OFFSET = TURN_EDU_ON,
  ID_RANGE = ID_END - ID_OFFSET
};

// Maps command strings to delegate IDs
auto const cmdStringToId = etl::make_map<etl::string<CMD_LENGTH_ETL>, CommandId>(
  etl::pair<etl::string<CMD_LENGTH_ETL>, CommandId>{"$01", TURN_EDU_ON},
  etl::pair<etl::string<CMD_LENGTH_ETL>, CommandId>{"$02", TURN_EDU_OFF},
  etl::pair<etl::string<CMD_LENGTH_ETL>, CommandId>{"$03", SEND_RESET_COUNTER},
  etl::pair<etl::string<CMD_LENGTH_ETL>, CommandId>{"$51", SEND_TEMPERATURE},
  etl::pair<etl::string<CMD_LENGTH_ETL>, CommandId>{"$52", SEND_IMAGE}
);

using CommandDelegateService = etl::delegate_service<ID_RANGE, ID_OFFSET>;

// Makes sure that callback service is initialised
CommandDelegateService& getCommandDelegateService()
{
  static CommandDelegateService commandDelegateService;
  return commandDelegateService;
}

class CommandParser : public StaticThread<>
{

  CommandDelegateService& commandDelegateService = getCommandDelegateService();

  void init() override
  {
    // Create delegates
    etl::delegate<void(size_t)> turnEduOnCallback = etl::delegate<void(size_t)>::create<turnEduOn>();
    etl::delegate<void(size_t)> turnEduOffCallback = etl::delegate<void(size_t)>::create<turnEduOff>();
    etl::delegate<void(size_t)> sendResetCounterCallback = etl::delegate<void(size_t)>::create<sendResetCounter>();
    etl::delegate<void(size_t)> sendTemperatureCallback = etl::delegate<void(size_t)>::create<sendTemperature>();
    etl::delegate<void(size_t)> sendImageCallback = etl::delegate<void(size_t)>::create<sendImage>();
    etl::delegate<void(size_t)> unhandledCommandCallback = etl::delegate<void(size_t)>::create<unhandledCommand>();

    // Register delegates with delegate_service
    commandDelegateService.register_delegate(TURN_EDU_ON, turnEduOnCallback);
    commandDelegateService.register_delegate(TURN_EDU_OFF, turnEduOffCallback);
    commandDelegateService.register_delegate(SEND_RESET_COUNTER, sendResetCounterCallback);
    commandDelegateService.register_delegate(SEND_TEMPERATURE, sendTemperatureCallback);
    commandDelegateService.register_delegate(SEND_IMAGE, sendImageCallback);
       
    // And one for invalid commands
    commandDelegateService.register_unhandled_delegate(unhandledCommandCallback);
  }

  void run() override
  {

    // Test run through the commands
    auto cmdArray = etl::make_array<etl::string<CMD_LENGTH_ETL>>("$01", "$02", "$03", "$51", "$52", "$22");
    PRINTF("%i\n", cmdArray.size());
    size_t i = 0;

    TIME_LOOP(0, 1 * SECONDS)
    {
      // FIXME: Why does the last element of the cmdArray cause a segfault when accessed by with i,
      // but not when accessed with an int literal? (Hence cmdArray.size() - 1)
      if (i >= cmdArray.size() - 1)
      {
        i = 0;
      }

      // Int literal check
      PRINTF("%s\n", cmdArray[5].c_str());

      CommandId cmdId = cmdStringToId.at(cmdArray.at(i));
      commandDelegateService.call(cmdId);
      i++;
    }
  
  }

};


auto const commandParser = CommandParser();

}

