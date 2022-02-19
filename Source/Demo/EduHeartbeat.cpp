#include <rodos.h>

auto const green = GPIO_005;
auto const eduHearbeatPin = GPIO_037;  // PC5


// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO eduHeartbeat(eduHearbeatPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO greenLed(green);

namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    eduHeartbeat.init(/*isOutput=*/false, 1, 0);
    greenLed.init(/*isOutput=*/true, 1, 0);
  }

  void run() override
  {
    auto heartBeatIsConstant = true;
    auto const treshold = 4;
    auto samplingCount = 0;
    auto eduHeartbeatPrevValue = eduHeartbeat.readPins();

    TIME_LOOP(0, 50 * MILLISECONDS)
    {
      auto eduHeartbeatvalue = eduHeartbeat.readPins();
      ++samplingCount;

      // NOLINTNEXTLINE
      PRINTF("EDU hearbeat value is : %lu \n", eduHeartbeatvalue);


      if(heartBeatIsConstant and (eduHeartbeatvalue != eduHeartbeatPrevValue))
      {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        PRINTF("EDU is alive !\n");
        heartBeatIsConstant = false;
        greenLed.setPins(1);
      }

      eduHeartbeatPrevValue = eduHeartbeatvalue;

      // Heartbeat is constant over the last 4* Sampling Period
      if(samplingCount == treshold)
      {
        if(heartBeatIsConstant)
        {
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
          PRINTF("EDU is dead !\n");
          greenLed.setPins(0);
        }
        heartBeatIsConstant = true;
        // reset timeout counter
        samplingCount = 0;
      }
    }
  }
};

auto const helloWorld = HelloWorld();
}
