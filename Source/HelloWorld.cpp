#include "Lib.hpp"

#include "rodos.h"

class hello_world : public StaticThread<>
{
  void run() override
  {
    auto const lib = library();
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

auto const hello = hello_world();
