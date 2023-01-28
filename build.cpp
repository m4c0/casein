#include "ecow.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  seq all { "all" };

  auto m = all.add_unit<per_feat<mod>>("casein");

  m->for_feature(android_ndk).add_impl("android");
  m->for_feature(android_ndk).add_unit<>("glue/android_native_app_glue.c");

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

  auto poc = all.add_unit<app>("poc");
  poc->add_unit<>("poc");
  poc->add_ref(m);

  return run_main(all, argc, argv);
}
