#include "Lib.hpp"

#include <rodos.h>


namespace rpg
{
constexpr auto eduUpdatePin = GPIO_017;  // PB01
auto blinkyPin = HAL_GPIO(eduUpdatePin);


class HelloWorld : public StaticThread<>
{
  void init() override
  {
    blinkyPin.init(/*isOutput=*/true);
    blinkyPin.setPins(1);
  }

  void run() override
  {
    auto const lib = Library();
    auto toggle = true;
    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      if(toggle)
      {
        blinkyPin.setPins(0);
      }
      else
      {
        blinkyPin.setPins(1);
      }
      toggle = !toggle;

      if(lib.name.length() > 0)
      {
        PRINTF("Hello World from %s!\n", lib.name.data());
      }
      else
      {
        PRINTF("Nothing to print!\n");
      }
    }
  }
};

auto const helloWorld = HelloWorld();
}
