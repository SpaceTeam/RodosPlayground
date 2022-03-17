target_sources(RodosPlayground_Beacon PRIVATE Beacon.cpp EduHeartbeat.cpp)
target_link_libraries(RodosPlayground_Beacon PRIVATE rodos::rodos etl)

target_sources(RodosPlayground_EduHeartbeat PRIVATE EduHeartbeat.cpp)
target_link_libraries(RodosPlayground_EduHeartbeat PRIVATE rodos::rodos)

target_sources(
    RodosPlayground_Demo PRIVATE Beacon.cpp EduHeartbeat.cpp CommandParser.cpp CobcCommands.cpp
                                 CobcCommands.hpp
)
target_link_libraries(RodosPlayground_Demo PRIVATE rodos::rodos etl)

target_sources(
    RodosPlayground_CommandParser PRIVATE CommandParser.cpp CobcCommands.cpp CobcCommands.hpp
)
target_link_libraries(RodosPlayground_CommandParser PRIVATE rodos::rodos etl)
