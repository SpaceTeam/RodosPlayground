#include "Lib.hpp"

#include <rodos.h>


namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void run() override
  {
    auto const library = Library();
    auto toggle = true;

    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      if(toggle)
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("Hello World from %s!\n", library.name.data());
      }
      else
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("Hello World from %s!\n", library.shortName.c_str());
      }
      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
