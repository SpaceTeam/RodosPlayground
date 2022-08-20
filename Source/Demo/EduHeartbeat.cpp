#include "Io.hpp"
#include "Topics.hpp"

#include <type_safe/types.hpp>

#include <rodos_no_using_namespace.h>


namespace rpg
{
auto ledGpio = RODOS::HAL_GPIO(ledPin);
auto heartbeatGpio = RODOS::HAL_GPIO(eduHeartbeatPin);


class EduHeartbeatThread : public RODOS::StaticThread<>
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
        namespace ts = type_safe;
        using ts::operator""_i;
        using ts::operator""_isize;

        auto samplingCount = 0_i;
        ts::bool_t heartbeatIsConstant = true;
        ts::uint32_t oldHeartbeat = heartbeatGpio.readPins();
        constexpr auto heartbeatFrequency = 10_isize;                     // Hz
        constexpr auto samplingFrequency = 5_isize * heartbeatFrequency;  // Hz
        constexpr auto samplingPeriode =
            1'000_isize / samplingFrequency * RODOS::MILLISECONDS;  // ms

        TIME_LOOP(0, samplingPeriode.get())
        {
            ts::uint32_t heartbeat = heartbeatGpio.readPins();
            ++samplingCount;

            if(heartbeatIsConstant and (heartbeat != oldHeartbeat))
            {
                heartbeatIsConstant = false;
                ledGpio.setPins(1U);
                eduIsAliveTopic.publish(true);
            }

            oldHeartbeat = heartbeat;

            // Check if heartbeat is constant over a whole heartbeat period
            if(samplingCount == samplingFrequency / heartbeatFrequency)
            {
                if(heartbeatIsConstant)
                {
                    ledGpio.setPins(0U);
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
