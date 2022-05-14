#pragma once

#include <type_safe/types.hpp>

#include <rodos.h>


namespace rpg
{
inline Topic<bool> eduIsAliveTopic(-1, "eduHeartBeatsTopic");
inline Topic<type_safe::int32_t> temperatureTopic(-1, "temperatureTopic");
inline Topic<type_safe::int32_t> accelerationXTopic(-1, "accelerationXTopic");
inline Topic<type_safe::int32_t> accelerationYTopic(-1, "accelerationYTopic");
inline Topic<type_safe::int32_t> accelerationZTopic(-1, "accelerationZTopic");
inline Topic<type_safe::int32_t> uvBrightnessTopic(-1, "uvBrightnessTopic");
}
