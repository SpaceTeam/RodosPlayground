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

    TIME_LOOP(0, 5000 * MILLISECONDS)
    {
      // greenLed.setPins(static_cast<uint32_t>(toggle));

      auto eduHeartbeatvalue = static_cast<int32_t>(eduHeartbeat.readPins());
      auto timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);
      // NOLINTNEXTLINE
      PRINTF("?%ld%ld\n", timestamp, eduHeartbeatvalue);
      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
