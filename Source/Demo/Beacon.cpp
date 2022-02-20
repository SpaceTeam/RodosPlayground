#include <rodos.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>

auto const eduHearbeatPin = GPIO_037;  // PC5

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO eduHeartbeat(eduHearbeatPin);

namespace RODOS
{
// NOLINTNEXTLINE(readability-identifier-naming, cppcoreguidelines-avoid-non-const-global-variables)
extern HAL_UART uart_stdout;
}


namespace rpg
{
class HelloWorld : public StaticThread<>
{
  void init() override
  {
    eduHeartbeat.init(/*isOutput=*/false, 1, 0);
  }

  static auto ConstructBeacon(int32_t timestamp, int32_t eduHeartbeatvalue)
  {
    auto const beaconSize = sizeof(eduHeartbeatvalue) + sizeof(timestamp) + sizeof('?');
    std::array<std::byte, beaconSize> beacon{};

    size_t i = 0;

    beacon[i] = static_cast<std::byte>('?');
    i += sizeof('?');
    std::memcpy(&beacon[i], &timestamp, sizeof(timestamp));
    i += sizeof(timestamp);
    std::memcpy(&beacon[i], &eduHeartbeatvalue, sizeof(timestamp));
    i += sizeof(eduHeartbeatvalue);

    return beacon;
  }

  void run() override
  {
    TIME_LOOP(0, 5000 * MILLISECONDS)
    {
      auto eduHeartbeatvalue = static_cast<int32_t>(eduHeartbeat.readPins());
      auto timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);

      auto beacon = ConstructBeacon(timestamp, eduHeartbeatvalue);

      uart_stdout.write(beacon.data(), beacon.size());
    }
  }
};

auto const helloWorld = HelloWorld();
}
