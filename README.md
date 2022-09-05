# RodosPlayground

[![codecov](https://codecov.io/gh/SpaceTeam/RodosPlayground/branch/catch2/graph/badge.svg?token=V4G6S8KDRX)](https://codecov.io/gh/SpaceTeam/RodosPlayground)

This is the RodosPlayground project.

## Building

The easiest way to build this project is with [Docker](https://www.docker.com/). The
[tuwienspaceteam/sts1-cobc](https://hub.docker.com/r/tuwienspaceteam/sts1-cobc) image is
specifically designed for that purpose. It comes with all required compilers, toolchains, libraries
and tools. If you don't want to cross-compile and are only interested in running the software and
tests on Linux than use the tags with a `-linux-x86` suffix. They are quite a bit smaller because
they do not have the GNU ARM Toolchain installed.

Even if you don't want to use Docker and instead build this project locally on your Linux machine
(Windows with WSL also works), looking at the two Dockerfiles
([here](https://github.com/SpaceTeam/STS1_COBC_Docker/blob/master/linux-x86/Dockerfile) and
[here](https://github.com/SpaceTeam/STS1_COBC_Docker/blob/master/full/Dockerfile)) is still the best
way to start. It shows exactly how to set up the compiler, toolchain, libraries and other things. We
also have the following two wiki pages to help you with your setup:

- [Setup: Compilers and Tools](https://wiki.tust.at/books/sts1/page/setup-compilers-and-tools)
- [Setup: Libraries](https://wiki.tust.at/books/sts1/page/setup-libraries)

After installing the compilers, tools and libraries you also need to setup a toolchain file and
presets.


### Toolchain file

Toolchain files are used when cross-compiling and are specific to your target platform, environment
and setup but not to your project. Therefore they are not supplied with this repo and should be kept
at some "global" directory (I, e.g., use `~/programming/cmake/`). The repo of the sts1-cobc Docker
image contains an appropriate [toolchain file targeting an
STM32F411](https://github.com/SpaceTeam/STS1_COBC_Docker/blob/master/full/stm32f411.cmake). You can
use this as a template for your own toolchain files. Just change the `platform_root` variable to
point to the directory where you install all your cross-compiled libraries for the target platform.


### Presets

This project makes use of [CMake
presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) to simplify the process of
configuring the project. As a developer, you should create a `CMakeUserPresets.json` file at the
root of the project that looks something like the following:

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
      "generator": "Ninja",
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
      "name": "dev-nucleo",
      "binaryDir": "${sourceDir}/build/nucleo",
      "inherits": "dev-common",
      "toolchainFile": "~/programming/cmake/stm32f411.cmake",
      "cacheVariables": {
        "RODOS_PACKAGE_NAME": "rodos_discovery_f411",
        "HSE_VALUE": "8000000",
        "BOARD_FILE": "Nucleo.hpp"
      }
    },
    {
      "name": "dev-cobc",
      "binaryDir": "${sourceDir}/build/cobc",
      "inherits": "dev-common",
      "toolchainFile": "~/programming/cmake/stm32f411.cmake",
      "cacheVariables": {
        "HSE_VALUE": "12000000",
        "BOARD_FILE": "Cobc.hpp"
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
      "jobs": 4
    },
    {
      "name": "dev-nucleo",
      "configurePreset": "dev-nucleo",
      "configuration": "Debug",
      "jobs": 4
    },
    {
      "name": "dev-cobc",
      "configurePreset": "dev-cobc",
      "configuration": "Debug",
      "jobs": 4
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
        "jobs": 4
      }
    }
  ]
}
```

The path to the toolchain files depend on your setup. The number of jobs given in the build and test
presets must be adapted by you as well and should ideally be set to the number of threads available
on your CPU.

In general, `CMakeUserPresets.json` is the perfect place in which you can put all sorts of things
that you would otherwise want to pass to the configure command in the terminal.


### Configure, build and test on Linux

If everything is set up properly, you can configure, build and test the project on Linux
respectively with the following commands from the project root:

```sh
cmake --preset=dev-linux-x86
cmake --build --preset=dev-linux-x86
ctest --preset=dev-linux-x86
```

To run the HelloWorld example execute

```sh
./build/linux-x86/HelloWorld
```

### Configure and build for as well as flash onto Nucleo-F411RE board

If everything is set up properly, you can configure and build the project for the Nucleo-F411RE
board respectively with the following commands from the project root:

```sh
cmake --preset=dev-nucleo
cmake --build --preset=dev-nucleo
```

To flash the example binary `HelloWorld.bin` onto the Nucleo board, copy it from `build/nucleo` to
the the storage device the board registers as when connected to a PC.

# Licensing

See the [LICENSE](LICENSE.md) document.
<!--
Please go to https://choosealicense.com/ and choose a license that fits your needs. GNU GPLv3 is a
pretty nice option ;-)
-->
