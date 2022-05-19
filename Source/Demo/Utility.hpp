#pragma once

#include <type_safe/types.hpp>

#include <rodos.h>

#include <etl/string.h>

#include <cstring>
#include <span>

#include <rodos-assert.h>


namespace rpg
{
namespace ts = type_safe;
using ts::operator""_isize;


constexpr auto beaconPeriod = 50_isize * MILLISECONDS;


auto CopyTo(std::span<std::byte> buffer, ts::size_t * const position, auto value)
{
    auto newPosition = *position + sizeof(value);
    RODOS_ASSERT_IFNOT_RETURN_VOID(newPosition <= std::size(buffer));
    std::memcpy(&buffer[(*position).get()], &value, sizeof(value));
    *position = newPosition;
}


template<std::size_t size>
auto CopyFrom(const etl::string<size> & buffer, ts::size_t * const position, auto * value)
{
    auto newPosition = *position + sizeof(*value);
    RODOS_ASSERT_IFNOT_RETURN_VOID(newPosition <= std::size(buffer));
    std::memcpy(value, &buffer[(*position).get()], sizeof(*value));
    *position = newPosition;
}
}
