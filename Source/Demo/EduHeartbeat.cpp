#include "Topics.hpp"

#include <rodos.h>


namespace rpg
{
constexpr auto pa5 = GPIO_013;
constexpr auto pc5 = GPIO_037;


auto greenLedGpio = HAL_GPIO(pa5);
auto heartbeatGpio = HAL_GPIO(pc5);


class EduHeartbeatThread : public StaticThread<>
{
public:
  EduHeartbeatThread() : StaticThread("EduHeartbeat")
  {
  }

private:
  void init() override
  {
    heartbeatGpio.init(/*isOutput=*/false, 1, 0);
    greenLedGpio.init(/*isOutput=*/true, 1, 0);
  }

  void run() override
  {
    auto heartbeatIsConstant = true;
    auto samplingCount = 0;
    auto oldHeartbeat = heartbeatGpio.readPins();
    constexpr auto heartbeatFrequency = 10LL;                                     // Hz
    constexpr auto samplingFrequency = 5LL * heartbeatFrequency;                  // Hz
    constexpr auto samplingPeriode = 1'000LL / samplingFrequency * MILLISECONDS;  // ms

    TIME_LOOP(0, samplingPeriode)
    {
      auto heartbeat = heartbeatGpio.readPins();
      ++samplingCount;

      if(heartbeatIsConstant and (heartbeat != oldHeartbeat))
      {
        heartbeatIsConstant = false;
        greenLedGpio.setPins(1);
        eduHeartbeatTopic.publish(static_cast<int32_t>(true));
      }

      oldHeartbeat = heartbeat;

      // Check if heartbeat is constant over a whole heartbeat periode
      if(samplingCount == samplingFrequency / heartbeatFrequency)
      {
        if(heartbeatIsConstant)
        {
          greenLedGpio.setPins(0);
          eduHeartbeatTopic.publish(static_cast<int32_t>(false));
        }
        heartbeatIsConstant = true;
        samplingCount = 0;
      }
    }
  }
};


auto const eduHeartbeatThread = EduHeartbeatThread();
}
