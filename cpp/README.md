# C++ Test FMUs

A simple framework that builds model exchange and co-simulation FMUs for FMI 2.0 and 3.0 from a model implemented in a single source file.

## Models

- `Arrays` - array inputs and outputs
- `BouncingBall` - two continuous states and state events
- `Stair` - one discrete variable and time events
- `VariableTypes` - all variable types

## Building the FMUs

- open the [CMake](https://cmake.org/) GUI

- supply the paths:

  Where is the source code: `/path/to/fmusdk/cpp`

  Where to build the binaries: `/path/to/fmusdk/cpp/build`

- select the FMI_VERSION you want (`2` or `3`)

- click `Configure` and select your toolchain then click `Generate` and `Open Project`

- build the solution / project

- go to `/path/to/fmusdk/cpp/dist` and simulate the FMUs
