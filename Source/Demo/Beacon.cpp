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
  }

  void run() override
  {
    auto toggle = true;

    TIME_LOOP(0, 1000 * MILLISECONDS)
    {
      greenLed.setPins(static_cast<uint32_t>(toggle));
      // auto eduHeartbeatvalue = eduHeartbeat.readPins();

      auto timestamp = NOW();
      // NOLINTNEXTLINE
      PRINTF("$%lld\n", timestamp);
      // PRINTF("$%lld%lu\n", timestamp, eduHeartbeatvalue);


      toggle = not toggle;
    }
  }
};

auto const helloWorld = HelloWorld();
}
