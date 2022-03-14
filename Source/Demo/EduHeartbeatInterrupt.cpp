#include <rodos.h>

auto const green = GPIO_005;
auto const eduHearbeatPin = GPIO_037;  // PC5


HAL_GPIO eduHeartbeat(eduHearbeatPin);
HAL_GPIO greenLed(green);

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    eduHeartbeat.init(/*isOutput=*/false, 1, 0);
    eduHeartbeat.interruptEnable(/*enable=*/true);
    greenLed.init(/*isOutput=*/true, 1, 0);
  }

  void run() override
  {
    constexpr auto timeout = 200;

    while(true)
    {
      PRINTF("Suspend until data is ready\n");
      eduHeartbeat.suspendUntilDataReady(NOW() + timeout * MILLISECONDS);
      if(eduHeartbeat.isDataReady())
      {
        PRINTF("Edu is alive \n");
        greenLed.setPins(1);
      }
      else
      {
        PRINTF("Edu is dead\n");
        greenLed.setPins(0);
      }
      eduHeartbeat.resetInterruptEventStatus();
    }
  }
};

auto const helloWorld = HelloWorld();
}
