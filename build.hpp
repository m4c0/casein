#pragma once
#include "../ecow/ecow.hpp"

inline auto casein() {
  using namespace ecow;

  auto m = unit::create<per_feat<mod>>("casein");

  m->for_feature(android_ndk).add_unit<>("android");
  m->for_feature(android_ndk).add_system_library("android");
  m->for_feature(android_ndk).add_system_library("log");

  m->for_feature(cocoa).add_unit<objc>("CASView");
  m->for_feature(cocoa).add_unit<objc>("osx");

  m->for_feature(uikit).add_unit<objc>("CASView");
  m->for_feature(uikit).add_unit<objc>("ios");

  m->for_feature(windows_api).add_unit<>("windows");
  m->for_feature(windows_api).add_system_library("user32");

  auto & wasm = m->for_feature(webassembly);
  wasm.add_unit<>("externc");
  wasm.add_unit<>("wasm");
  wasm.add_feat<setup_js>("casein");

  return m;
}
