#include <rodos.h>

#include <array>


namespace rpg
{
// TODO: Put this in a separat file like PinNames.hpp, PinDeclarations.hpp, Pins.hpp or something
constexpr auto PA3 = GPIO_003;   // NOLINT(readability-identifier-naming)
constexpr auto PA5 = GPIO_005;   // NOLINT(readability-identifier-naming)
constexpr auto PA6 = GPIO_006;   // NOLINT(readability-identifier-naming)
constexpr auto PA7 = GPIO_007;   // NOLINT(readability-identifier-naming)
constexpr auto PC4 = GPIO_036;   // NOLINT(readability-identifier-naming)
constexpr auto PC5 = GPIO_037;   // NOLINT(readability-identifier-naming)
constexpr auto PB0 = GPIO_016;   // NOLINT(readability-identifier-naming)
constexpr auto PB1 = GPIO_017;   // NOLINT(readability-identifier-naming)
constexpr auto PB12 = GPIO_028;  // NOLINT(readability-identifier-naming)
constexpr auto PB13 = GPIO_029;  // NOLINT(readability-identifier-naming)
constexpr auto PB14 = GPIO_030;  // NOLINT(readability-identifier-naming)
constexpr auto PB15 = GPIO_031;  // NOLINT(readability-identifier-naming)
constexpr auto PC7 = GPIO_039;   // NOLINT(readability-identifier-naming)
constexpr auto PC9 = GPIO_041;   // NOLINT(readability-identifier-naming)
constexpr auto PA8 = GPIO_008;   // NOLINT(readability-identifier-naming)
constexpr auto PA9 = GPIO_009;   // NOLINT(readability-identifier-naming)
constexpr auto PA10 = GPIO_010;  // NOLINT(readability-identifier-naming)
constexpr auto PA11 = GPIO_011;  // NOLINT(readability-identifier-naming)
constexpr auto PA12 = GPIO_012;  // NOLINT(readability-identifier-naming)
constexpr auto PA13 = GPIO_013;  // NOLINT(readability-identifier-naming)
constexpr auto PA14 = GPIO_014;  // NOLINT(readability-identifier-naming)
constexpr auto PA15 = GPIO_015;  // NOLINT(readability-identifier-naming)
constexpr auto PC10 = GPIO_042;  // NOLINT(readability-identifier-naming)
constexpr auto PC11 = GPIO_043;  // NOLINT(readability-identifier-naming)
constexpr auto PC12 = GPIO_044;  // NOLINT(readability-identifier-naming)
constexpr auto PD2 = GPIO_050;   // NOLINT(readability-identifier-naming)
constexpr auto PB3 = GPIO_019;   // NOLINT(readability-identifier-naming)
constexpr auto PB4 = GPIO_020;   // NOLINT(readability-identifier-naming)
constexpr auto PB5 = GPIO_021;   // NOLINT(readability-identifier-naming)
constexpr auto PB6 = GPIO_022;   // NOLINT(readability-identifier-naming)
constexpr auto PB7 = GPIO_023;   // NOLINT(readability-identifier-naming)
constexpr auto PB8 = GPIO_024;   // NOLINT(readability-identifier-naming)
constexpr auto PB9 = GPIO_025;   // NOLINT(readability-identifier-naming)
constexpr auto PC13 = GPIO_045;  // NOLINT(readability-identifier-naming)
constexpr auto PC0 = GPIO_032;   // NOLINT(readability-identifier-naming)
constexpr auto PC1 = GPIO_033;   // NOLINT(readability-identifier-naming)
constexpr auto PC2 = GPIO_034;   // NOLINT(readability-identifier-naming)
constexpr auto PC3 = GPIO_035;   // NOLINT(readability-identifier-naming)
constexpr auto PA1 = GPIO_001;   // NOLINT(readability-identifier-naming)
constexpr auto PA2 = GPIO_002;   // NOLINT(readability-identifier-naming)


// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto pinsToTest = std::array{
  HAL_GPIO(PA3),  HAL_GPIO(PA5),  HAL_GPIO(PA6),  HAL_GPIO(PA7),  HAL_GPIO(PC4),  HAL_GPIO(PC5),
  HAL_GPIO(PB0),  HAL_GPIO(PB1),  HAL_GPIO(PB12), HAL_GPIO(PB13), HAL_GPIO(PB14), HAL_GPIO(PB15),
  HAL_GPIO(PC7),  HAL_GPIO(PC9),  HAL_GPIO(PA8),  HAL_GPIO(PA9),  HAL_GPIO(PA10), HAL_GPIO(PA11),
  HAL_GPIO(PA12), HAL_GPIO(PA13), HAL_GPIO(PA14), HAL_GPIO(PA15), HAL_GPIO(PC10), HAL_GPIO(PC11),
  HAL_GPIO(PC12), HAL_GPIO(PD2),  HAL_GPIO(PB3),  HAL_GPIO(PB4),  HAL_GPIO(PB5),  HAL_GPIO(PB6),
  HAL_GPIO(PB7),  HAL_GPIO(PB8),  HAL_GPIO(PB9),  HAL_GPIO(PC13), HAL_GPIO(PC0),  HAL_GPIO(PC1),
  HAL_GPIO(PC2),  HAL_GPIO(PC3),  HAL_GPIO(PA1),  HAL_GPIO(PA2)};


class HelloGpio : public StaticThread<>
{
  void init() override
  {
    for(auto & pin : pinsToTest)
    {
      pin.init(/*isOutput=*/true, 1, 0);
    }
  }

  void run() override
  {
    auto toggle = true;

    TIME_LOOP(0, 500 * MILLISECONDS)
    {
      for(auto & i : pinsToTest)
      {
        i.setPins(static_cast<uint32_t>(toggle));
      }
      toggle = not toggle;
    }
  }
};


auto const helloGpio = HelloGpio();
}
