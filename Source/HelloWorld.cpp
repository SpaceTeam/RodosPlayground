#include "Lib.hpp"
#include "rodos.h"

class hello_world : public StaticThread<>
{
  void run() override
  {
    auto const lib = library();
    PRINTF("Hello World from %s!\n", lib.name.data());
  }
};

auto const hello = hello_world();
