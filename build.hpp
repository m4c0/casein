#pragma once
#include "ecow.hpp"

inline auto casein() {
  using namespace ecow;

  auto m = unit::create<per_feat<mod>>("casein");

  m->for_feature(android_ndk).add_unit<>("android");

  m->for_feature(cocoa).add_unit<objc>("osx");
  m->for_feature(cocoa).add_unit<>("externc");

  m->for_feature(uikit).add_unit<objc>("ios");
  m->for_feature(uikit).add_unit<>("externc");

  m->for_feature(windows_api).add_unit<>("windows");
  m->for_feature(windows_api).add_system_library("user32");

  m->for_feature(webassembly).add_unit<>("externc");

  auto mwjs = m->for_feature(webassembly).add_feat<js>();
  mwjs->set("casein_create_window", "");
  mwjs->set("casein_repaint", "");
  mwjs->set("casein_key_down", "");
  mwjs->set("casein_key_up", "");
  mwjs->set("casein_mouse_down", "");
  mwjs->set("casein_mouse_move", "");
  mwjs->set("casein_mouse_up", "");

  return m;
}
