#include <rodos.h>

#include "Topics.h"

constexpr auto greenLedPin = GPIO_005;     // PA5
constexpr auto eduHearbeatPin = GPIO_037;  // PC5

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto eduHeartbeat = HAL_GPIO(eduHearbeatPin);
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto greenLed = HAL_GPIO(greenLedPin);

namespace rpg
{
class EduHeartbeat : public StaticThread<>
{
public:
  EduHeartbeat() : StaticThread("EduHeartbeat")
  {
  }

private:
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

      if(heartBeatIsConstant and (eduHeartbeatvalue != eduHeartbeatPrevValue))
      {
        heartBeatIsConstant = false;
        greenLed.setPins(1);
        eduHeartbeatTopic.publish(static_cast<int32_t>(true));
      }

      eduHeartbeatPrevValue = eduHeartbeatvalue;

      // Heartbeat is constant over the last 4* Sampling Period
      if(samplingCount == treshold)
      {
        if(heartBeatIsConstant)
        {
          greenLed.setPins(0);
          eduHeartbeatTopic.publish(static_cast<int32_t>(false));
        }
        heartBeatIsConstant = true;
        // reset timeout counter
        samplingCount = 0;
      }
    }
  }
};

auto const eduHeartbeat = EduHeartbeat();
}
