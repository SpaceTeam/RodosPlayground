target_sources(RodosPlayground_Beacon PRIVATE Beacon.cpp EduHeartbeat.cpp)
target_link_libraries(
    RodosPlayground_Beacon PRIVATE rodos::rodos etl Io_Lib RodosWrappers_Lib type_safe
)

target_sources(RodosPlayground_EduHeartbeat PRIVATE EduHeartbeat.cpp)
target_link_libraries(RodosPlayground_EduHeartbeat PRIVATE rodos::rodos Io_Lib type_safe)

target_sources(RodosPlayground_EduData PRIVATE EduData.cpp)
target_link_libraries(
    RodosPlayground_EduData PRIVATE rodos::rodos Io_Lib type_safe RodosWrappers_Lib
)

target_sources(
    RodosPlayground_Demo
    PRIVATE Beacon.cpp
            EduHeartbeat.cpp
            EduData.cpp
            CommandParser.cpp
            CobcCommands.cpp
            CobcCommands.hpp
            Utility.hpp
            Beacon.hpp
)
target_link_libraries(
    RodosPlayground_Demo PRIVATE rodos::rodos etl::etl Io_Lib RodosWrappers_Lib type_safe
)

target_sources(
    RodosPlayground_CommandParser PRIVATE CommandParser.cpp CobcCommands.cpp CobcCommands.hpp
)
target_link_libraries(
    RodosPlayground_CommandParser PRIVATE rodos::rodos etl::etl Io_Lib RodosWrappers_Lib type_safe
)

target_sources(RodosPlayground_EduDataFrameFaker PRIVATE EduDataFrameFaker.cpp)
target_link_libraries(
    RodosPlayground_EduDataFrameFaker PRIVATE rodos::rodos Io_Lib RodosWrappers_Lib type_safe
)
