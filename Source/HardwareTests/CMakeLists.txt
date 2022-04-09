target_sources(RodosPlayground_HelloUart PRIVATE HelloUart.cpp)
target_link_libraries(RodosPlayground_HelloUart PRIVATE rodos::rodos etl Io_Lib RodosWrappers_Lib)

target_sources(RodosPlayground_HelloGpio PRIVATE HelloGpio.cpp)
target_link_libraries(RodosPlayground_HelloGpio PRIVATE rodos::rodos Io_Lib)

target_sources(RodosPlayground_HelloSpi PRIVATE HelloSpi.cpp)
target_link_libraries(RodosPlayground_HelloSpi PRIVATE rodos::rodos etl Io_Lib RodosWrappers_Lib type_safe)
