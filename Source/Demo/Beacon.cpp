#include <rodos.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>

auto const green = GPIO_005;
auto const eduHearbeatPin = GPIO_037;  // PC5

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
HAL_GPIO greenLed(green);
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
    greenLed.init(/*isOutput=*/true, 1, 0);
    eduHeartbeat.init(/*isOutput=*/false, 1, 0);
  }

  void run() override
  {
    TIME_LOOP(0, 5000 * MILLISECONDS)
    {
      // Fetch values
      auto eduHeartbeatvalue = static_cast<int32_t>(eduHeartbeat.readPins());
      auto timestamp = static_cast<int32_t>(NOW() / MILLISECONDS);

      auto const beaconSize = sizeof(eduHeartbeatvalue) + sizeof(timestamp) + sizeof('?');
      std::array<std::byte, beaconSize> beacon{};

      Serialize(beacon, timestamp, eduHeartbeatvalue);

      uart_stdout.write(beacon.data(), beacon.size());
    }
  }

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  static void Serialize(std::array<std::byte, 9> beacon,
                        int32_t timestamp,
                        int32_t eduHeartbeatvalue)
  {
    beacon[0] = static_cast<std::byte>('?');
    std::memcpy(&beacon[1], &timestamp, sizeof(timestamp));
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    std::memcpy(&beacon[5], &eduHeartbeatvalue, sizeof(timestamp));
  }
};

auto const helloWorld = HelloWorld();
}
