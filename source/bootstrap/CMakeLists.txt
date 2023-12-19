## General CMake setup routine...

cmake_minimum_required(VERSION 3.25)
project(kart CXX)

## Support for modern C/C++ standards

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

## Glob all the source files for executable
file(
  GLOB_RECURSE
  KART_SOURCES
  "**.c"
  "**.cpp"
)

## Create executable
add_executable(
  ${PROJECT_NAME}
  ${KART_SOURCES}
)
