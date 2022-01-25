#include "Lib.hpp"

#include <rodos.h>


namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void run() override
  {
    auto const lib = Library();
    TIME_LOOP(0, 1 * RODOS::SECONDS)
    if(lib.name.length() > 0)
    {
      PRINTF("Hello World from %s!\n", lib.name.data());
    }
    else
    {
      PRINTF("Nothing to print!\n");
    }
  }
};

auto const helloWorld = HelloWorld();
}
