cmake_minimum_required(VERSION 3.20.4)
project(casein
  VERSION 1.0.0
  DESCRIPTION "C++ boilerplate library for UI-based multiplatform apps"
  LANGUAGES C CXX)

# Harmless but useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(casein STATIC)
target_sources(casein PUBLIC src/windows.cpp)

# Proof-of-concept, should be a nice way of understanding how to user this
# library
add_executable(casein-poc WIN32)
target_link_libraries(casein-poc PUBLIC casein)
