cmake_minimum_required(VERSION 3.23)
project(casein
  VERSION 1.0.0
  DESCRIPTION "C++ boilerplate library for UI-based multiplatform apps"
  LANGUAGES C CXX)

# Harmless but useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#####################################################
# Definitions, shared between casein implementations
#####################################################

add_library(casein-defs INTERFACE)
target_compile_features(casein-defs INTERFACE cxx_std_20)
target_compile_options(casein-defs INTERFACE
  $<$<PLATFORM_ID:Darwin>:-fobjc-arc>
  $<$<PLATFORM_ID:iOS>:-fobjc-arc>)
target_link_libraries(casein-defs INTERFACE
  $<$<PLATFORM_ID:Android>:android log>
  "$<$<PLATFORM_ID:Darwin>:-framework Cocoa>"
  "$<$<PLATFORM_ID:iOS>:-framework Foundation>"
  "$<$<PLATFORM_ID:iOS>:-framework UIKit>")
target_link_options(casein-defs INTERFACE
  $<$<PLATFORM_ID:Android>:-u ANativeActivity_onCreate>
  $<$<PLATFORM_ID:iOS>:-ObjC>)
install(TARGETS casein-defs EXPORT casein-targets)

#####################################
# Base library, minimal requirements
#####################################

add_library(casein STATIC)
target_link_libraries(casein PUBLIC casein-defs)
target_sources(casein PRIVATE
  $<$<PLATFORM_ID:Android>:glue/android_native_app_glue.c>
  $<$<PLATFORM_ID:Android>:android.cpp>
  $<$<PLATFORM_ID:Darwin>:osx.mm>
  $<$<PLATFORM_ID:iOS>:ios.mm>
  $<$<PLATFORM_ID:Windows>:windows.cpp>)
target_sources(casein PUBLIC FILE_SET HEADERS FILES
  casein.hpp
  $<$<PLATFORM_ID:Windows>:casein.windows.hpp>)

install(TARGETS casein EXPORT casein-targets FILE_SET HEADERS)
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
target_sources(casein-poc PRIVATE poc.cpp)

################################################################
# SDL2 wrapper and its POC. Only available if SDL2 is available
################################################################

find_package(SDL2)
if(SDL2_FOUND)
  add_library(casein-sdl STATIC)
  target_link_libraries(casein-sdl PUBLIC casein-defs SDL2::SDL2-static SDL2::SDL2main)
  target_sources(casein-sdl PRIVATE sdl.cpp)
  install(TARGETS casein-sdl EXPORT casein-targets FILE_SET HEADERS)

  if(WIN32 OR APPLE)
    add_executable(casein-sdl-poc MACOSX_BUNDLE WIN32)
    target_link_libraries(casein-sdl-poc PUBLIC casein-sdl)
    target_sources(casein-sdl-poc PRIVATE poc.sdl.cpp)
  endif()
endif()

