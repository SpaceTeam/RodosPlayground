#include <rodos.h>

#include <array>


namespace rpg
{
// TODO: Put this in a separat file like PinNames.hpp, PinDeclarations.hpp, Pins.hpp or something
constexpr auto pa1 = GPIO_001;
constexpr auto pa2 = GPIO_002;
constexpr auto pa3 = GPIO_003;
constexpr auto pa5 = GPIO_005;
constexpr auto pa6 = GPIO_006;
constexpr auto pa7 = GPIO_007;
constexpr auto pa8 = GPIO_008;
constexpr auto pa9 = GPIO_009;
constexpr auto pa10 = GPIO_010;
constexpr auto pa11 = GPIO_011;
constexpr auto pa12 = GPIO_012;
constexpr auto pa13 = GPIO_013;
constexpr auto pa14 = GPIO_014;
constexpr auto pa15 = GPIO_015;
constexpr auto pb0 = GPIO_016;
constexpr auto pb1 = GPIO_017;
constexpr auto pb3 = GPIO_019;
constexpr auto pb4 = GPIO_020;
constexpr auto pb5 = GPIO_021;
constexpr auto pb6 = GPIO_022;
constexpr auto pb7 = GPIO_023;
constexpr auto pb8 = GPIO_024;
constexpr auto pb9 = GPIO_025;
constexpr auto pb12 = GPIO_028;
constexpr auto pb13 = GPIO_029;
constexpr auto pb14 = GPIO_030;
constexpr auto pb15 = GPIO_031;
constexpr auto pc0 = GPIO_032;
constexpr auto pc1 = GPIO_033;
constexpr auto pc2 = GPIO_034;
constexpr auto pc3 = GPIO_035;
constexpr auto pc4 = GPIO_036;
constexpr auto pc5 = GPIO_037;
constexpr auto pc7 = GPIO_039;
constexpr auto pc9 = GPIO_041;
constexpr auto pc10 = GPIO_042;
constexpr auto pc11 = GPIO_043;
constexpr auto pc12 = GPIO_044;
constexpr auto pc13 = GPIO_045;
constexpr auto pd2 = GPIO_050;


// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
auto pinsToTest = std::to_array<HAL_GPIO>(
  {pa1, pa2, pa3, pa5, pa6, pa7, pa8, pa9,  pa10, pa11, pa12, pa13, pa14, pa15,
   pb0, pb1, pb3, pb4, pb5, pb6, pb7, pb8,  pb9,  pb12, pb13, pb14, pb15, pc0,
   pc1, pc2, pc3, pc4, pc5, pc7, pc9, pc10, pc11, pc12, pc13, pd2});


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

    TIME_LOOP(0, 100 * MILLISECONDS)
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
