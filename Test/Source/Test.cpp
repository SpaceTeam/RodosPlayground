#include "Utility.hpp"

#include <type_safe/types.hpp>

#include <catch2/catch_test_macros.hpp>


namespace ts = type_safe;
using ts::operator""_usize;

// NOLINTNEXTLINE (misc-no-recursion)
auto Factorial(unsigned int number) -> unsigned int
{
    return number > 1 ? Factorial(number - 1) * number : 1;
}


TEST_CASE("Factorials are computed 1", "[factorial]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}


TEST_CASE("Factorials are computed 2", "[factorial]")
{
    REQUIRE(Factorial(0) == 1);
}


TEST_CASE("CopyTo Position Offset", "[Utility]")
{
    constexpr auto beaconSize = 2;
    constexpr auto startByte = '?';
    auto beacon = std::array<std::byte, beaconSize>{};
    auto position = 0_usize;
    rpg::util::CopyTo(beacon, &position, startByte);
    REQUIRE(position.get() == sizeof(startByte));
}
