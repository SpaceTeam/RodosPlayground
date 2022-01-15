#include "Lib.hpp"

#include <rodos.h>

class HelloWorld : public StaticThread<>
{
  void run() override
  {
    auto const lib = Library();
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
