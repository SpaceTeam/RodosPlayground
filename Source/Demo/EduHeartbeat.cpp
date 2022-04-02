#include "Io.hpp"
#include "Topics.hpp"

#include <rodos.h>

namespace rpg
{
auto ledGpio = HAL_GPIO(ledPin);
auto heartbeatGpio = HAL_GPIO(eduHeartbeatPin);

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
    ledGpio.init(/*isOutput=*/true, 1, 0);
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
        ledGpio.setPins(1);
        eduIsAliveTopic.publish(true);
      }

      oldHeartbeat = heartbeat;

      // Check if heartbeat is constant over a whole heartbeat periode
      if(samplingCount == samplingFrequency / heartbeatFrequency)
      {
        if(heartbeatIsConstant)
        {
          ledGpio.setPins(0);
          eduIsAliveTopic.publish(false);
        }
        heartbeatIsConstant = true;
        samplingCount = 0;
      }
    }
  }
};


auto const eduHeartbeatThread = EduHeartbeatThread();
}
