cmake_minimum_required(VERSION 3.20.4)
project(casein
  VERSION 1.0.0
  DESCRIPTION "C++ boilerplate library for UI-based multiplatform apps"
  LANGUAGES C CXX)

# Harmless but useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(casein STATIC)
target_compile_features(casein PUBLIC cxx_std_17)
target_compile_options(casein PUBLIC
  $<$<PLATFORM_ID:Darwin>:-fobjc-arc>
  $<$<PLATFORM_ID:iOS>:-fobjc-arc>)
target_include_directories(casein PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(casein PUBLIC
  $<$<PLATFORM_ID:Android>:android log>
  "$<$<PLATFORM_ID:Darwin>:-framework Cocoa>"
  "$<$<PLATFORM_ID:iOS>:-framework UIKit>")
target_link_options(casein PUBLIC 
  $<$<PLATFORM_ID:Android>:-u ANativeActivity_onCreate>)
target_sources(casein PRIVATE
  $<$<PLATFORM_ID:Android>:src/glue/android_native_app_glue.c>
  $<$<PLATFORM_ID:Android>:src/android.cpp>
  $<$<PLATFORM_ID:Darwin>:src/osx.mm>
  $<$<PLATFORM_ID:iOS>:src/ios.mm>
  $<$<PLATFORM_ID:Windows>:src/windows.cpp>)
install(DIRECTORY inc/ TYPE INCLUDE)
install(TARGETS casein EXPORT casein-targets)
install(EXPORT casein-targets DESTINATION cmake/casein)
install(FILES casein-config.cmake DESTINATION cmake/casein)

###########################################################################
# Proof-of-concept, should be a nice way of understanding how to user this
# library
###########################################################################

if(ANDROID)
  # Casein assumes the usage of NativeActivity, which in turn requires a shared
  # library, insted of a executable
  add_library(casein-poc SHARED)
else()
  add_executable(casein-poc MACOSX_BUNDLE WIN32)
endif()
target_link_libraries(casein-poc PUBLIC casein)
target_sources(casein-poc PRIVATE poc/main.cpp)

