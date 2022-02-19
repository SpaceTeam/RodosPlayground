#include <rodos.h>

auto const green = GPIO_005;
auto const eduHearbeatPin = GPIO_037;  // PC5

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO greenLed(green);

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO eduHeartbeat(eduHearbeatPin);

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    greenLed.init(/*isOutput=*/true, 1, 0);
    eduHeartbeat.init(/*isOutput=*/false, 1, 0);
  }

  void run() override
  {
    auto toggle = true;
    auto hearbeatChanges = 0;
    auto const treshold = 3;
    auto eduHeartbeatPrevValue = eduHeartbeat.readPins();

    TIME_LOOP(0, 1500 * MILLISECONDS)
    {
      greenLed.setPins(static_cast<uint32_t>(toggle));
      auto eduHeartbeatvalue = eduHeartbeat.readPins();

      // NOLINTNEXTLINE
      PRINTF("EDU hearbeat value is : %lu \n", eduHeartbeatvalue);

      // Get hearbeat every x seconds.
      // If hearbeat is different from previous, increment a counter.
      // When counter is superior to manually defined treshold, print
      // that edu is alive to the standart output.

      if(eduHeartbeatvalue != eduHeartbeatPrevValue)
      {
        ++hearbeatChanges;
      }

      if(hearbeatChanges > treshold)
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("EDU is alive !\n");
      }

      eduHeartbeatPrevValue = eduHeartbeatvalue;

      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
