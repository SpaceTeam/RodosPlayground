if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/RodosPlayground CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package RodosPlayground)

install(
    TARGETS RodosPlayground_exe
    EXPORT RodosPlaygroundTargets
    RUNTIME COMPONENT RodosPlayground_Runtime
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    RodosPlayground_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(RodosPlayground_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${RodosPlayground_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT RodosPlayground_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${RodosPlayground_INSTALL_CMAKEDIR}"
    COMPONENT RodosPlayground_Development
)

install(
    EXPORT RodosPlaygroundTargets
    NAMESPACE RodosPlayground::
    DESTINATION "${RodosPlayground_INSTALL_CMAKEDIR}"
    COMPONENT RodosPlayground_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
