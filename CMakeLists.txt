cmake_minimum_required(VERSION 3.20.4)
project(casein
  VERSION 1.0.0
  DESCRIPTION "C++ boilerplate library for UI-based multiplatform apps"
  LANGUAGES C CXX)

# Harmless but useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(casein STATIC)
target_compile_features(casein PUBLIC cxx_std_17)
target_include_directories(casein PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_sources(casein PRIVATE
  $<$<PLATFORM_ID:Windows>:src/windows.cpp>)
install(DIRECTORY inc/ TYPE INCLUDE)
install(TARGETS casein EXPORT casein-targets)
install(EXPORT casein-targets DESTINATION cmake/casein)

# Proof-of-concept, should be a nice way of understanding how to user this
# library
add_executable(casein-poc WIN32 poc/main.cpp)
target_link_libraries(casein-poc PUBLIC casein)
