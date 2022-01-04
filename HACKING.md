# Hacking

Here is some wisdom to help you build and test this project as a developer and
potential contributor. It is assumed, that you are developing on Linux.

If you plan to contribute, please read the [CONTRIBUTING](CONTRIBUTING.md)
guide.

## Developer mode

Build system targets that are only useful for developers of this project are
hidden if the `RodosPlayground_DEVELOPER_MODE` option is disabled. Enabling this
option makes tests and other developer targets and options available. Not
enabling this option means that you are a consumer of this project and thus you
have no need for these targets and options.

Developer mode is always set to "ON" in CI workflows.

### Presets

This project makes use of [presets][1] to simplify the process of configuring
the project. As a developer, you are recommended to always have the [latest
CMake version][2] installed to make use of the latest Quality-of-Life
additions.

You have a few options to pass `RodosPlayground_DEVELOPER_MODE` to the configure
command, but this project prefers to use presets.

As a developer, you should create a `CMakeUserPresets.json` file at the root of
the project that looks something like the following:

```json
{
  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 22,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev-common",
      "hidden": true,
      "inherits": ["dev-mode", "cppcheck", "clang-tidy", "unix-common"],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "BUILD_MCSS_DOCS": "ON"
      }
    },
    {
      "name": "dev-linux-x86",
      "binaryDir": "${sourceDir}/build/linux-x86",
      "inherits": "dev-common",
      "toolchainFile": "/usr/local/src/rodos/cmake/port/linux-x86.cmake"
    },
    {
      "name": "dev-discovery_f429",
      "binaryDir": "${sourceDir}/build/discovery_f429",
      "inherits": "dev-common",
      "toolchainFile": "/usr/local/stm32f4/src/rodos/cmake/port/discovery_f429.cmake",
      "cacheVariables": {
        "CMAKE_FIND_ROOT_PATH": "/usr/local/stm32f4"
      }
    },
    {
      "name": "dev-coverage",
      "inherits": ["dev-mode", "coverage-unix"],
      "toolchainFile": "/usr/local/src/rodos/cmake/port/linux-x86.cmake"
    }
  ],
  "buildPresets": [
    {
      "name": "dev-linux-x86",
      "configurePreset": "dev-linux-x86",
      "configuration": "Debug",
      "jobs": 8
    }
  ],
  "testPresets": [
    {
      "name": "dev-linux-x86",
      "configurePreset": "dev-linux-x86",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      },
      "execution": {
        "jobs": 8
      }
    }
  ]
}
```

The exact paths of the toolchain files and the `CMAKE_FIND_ROOT_PATH` variable
depend on the install location of Rodos on your machine. The number of jobs
given in the build and test presets must be adapted by you as well and should
ideally be set to the number of threads available on your CPU.

In general, `CMakeUserPresets.json` is the perfect place in which you can put
all sorts of things that you would otherwise want to pass to the configure
command in the terminal.

### Configure, build and test

If you followed the above instructions, then you can configure, build and test
the project respectively with the following commands from the project root:

```sh
cmake --preset=dev-linux-x86
cmake --build --preset=dev-linux-x86
ctest --preset=dev-linux-x86
```

## To-dos

- Add something about `objcopy`
- Maybe add something about how to flash


[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
[2]: https://cmake.org/download/
