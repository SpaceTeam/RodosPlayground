if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/rodos_playground CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package rodos_playground)

install(
    TARGETS rodos_playground_rodos_playground
    EXPORT rodos_playgroundTargets
    RUNTIME COMPONENT rodos_playground_Runtime
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    rodos_playground_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(rodos_playground_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${rodos_playground_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT rodos_playground_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${rodos_playground_INSTALL_CMAKEDIR}"
    COMPONENT rodos_playground_Development
)

install(
    EXPORT rodos_playgroundTargets
    NAMESPACE rodos_playground::
    DESTINATION "${rodos_playground_INSTALL_CMAKEDIR}"
    COMPONENT rodos_playground_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
