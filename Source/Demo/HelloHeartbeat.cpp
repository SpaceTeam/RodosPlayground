#include <rodos.h>

auto const green = GPIO_005;
auto const eduHearbeatPin = GPIO_037;  // PC5

// NOLINTNEXTLINE
HAL_GPIO greenLed(green);
// NOLINTNEXTLINE
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

    TIME_LOOP(0, 1500 * MILLISECONDS)
    {
      greenLed.setPins(static_cast<uint32_t>(toggle));
      auto eduHeartbeatvalue = eduHeartbeat.readPins();

      // NOLINTNEXTLINE
      PRINTF("EDU hearbeat value is : %lu\n", eduHeartbeatvalue);

      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
