#include <type_safe/types.hpp>

#include <rodos.h>

namespace rpg
{
namespace ts = type_safe;
using ts::operator""_isize;

constexpr auto beaconPeriod = 50_isize * MILLISECONDS;
}
