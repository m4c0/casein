module;

#ifdef __APPLE__
typedef struct CAMetalLayer casein_native_handle;
#elif defined(_WIN32)
typedef struct HWND__ casein_native_handle;
#elif defined(__ANDROID__)
typedef struct ANativeWindow casein_native_handle;
#elif defined(__wasm__)
using casein_native_handle = void;
#elif __linux__
struct casein_native_handle;
#endif

export module casein;
import dotz;
import hai;
import jute;

export {
#include "common.hpp"
}

export namespace casein {
  extern dotz::vec2 window_size;
  extern dotz::vec2 mouse_pos;
  extern dotz::vec2 mouse_rel;
  extern float screen_scale_factor;
  extern bool keydown_repeating;
  extern hai::varray<hai::cstr> dropped_files;

  void set_title(const char * title);

  // Signal the app to eventually exit.
  void exit(int code);

  bool is_fullscreen();
  void set_fullscreen(bool);

  void enable_file_drop(bool);

  void handle(event_type, void (*)());
  void handle(event_type, keys, void (*)());
  void handle(event_type, mouse_buttons, void (*)());
  void handle(event_type, gestures, void (*)());

  // Resets a sub-map. Should be useful for context-aware input handling (ex: modals, etc)
  void reset_k(event_type);
  void reset_m(event_type);
  void reset_g(event_type);

  using native_handle_t = ::casein_native_handle *;
  extern native_handle_t native_ptr;
}

module :private;

extern "C" void casein_exit(int);
void casein::exit(int code) {
  casein_exit(code);
}

extern "C" void casein_set_title(const char *);
void casein::set_title(const char * title) {
  casein_set_title(title);
}

extern "C" void casein_enter_fullscreen();
extern "C" void casein_leave_fullscreen();
bool in_fullscreen {};
bool casein::is_fullscreen() {
  return in_fullscreen;
}
void casein::set_fullscreen(bool f) {
  if (in_fullscreen == f) return;
  in_fullscreen = f;
  if (f) {
    casein_enter_fullscreen();
  } else {
    casein_leave_fullscreen();
  }
}

extern "C" void casein_enable_filedrop(bool);
void casein::enable_file_drop(bool e) {
  casein_enable_filedrop(e);
}

extern "C" void casein_add_drop(const char * str, unsigned len) {
  jute::view v { str, len };
  casein::dropped_files.push_back_doubling(v.cstr());
}
extern "C" void casein_clear_drops() {
  casein::dropped_files.truncate(0);
}

casein::native_handle_t casein::native_ptr;
dotz::vec2 casein::mouse_pos;
dotz::vec2 casein::mouse_rel;
float casein::screen_scale_factor = 1.0;
bool casein::keydown_repeating;
dotz::vec2 casein::window_size;
hai::varray<hai::cstr> casein::dropped_files {};

#pragma leco add_impl handler

#ifdef LECO_TARGET_MACOSX
#pragma leco add_impl CASView osx
#pragma leco add_framework AppKit MetalKit Metal
#elif LECO_TARGET_IOS
#pragma leco add_impl CASView ios
#pragma leco add_framework MetalKit Metal UIKit
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#pragma leco add_library user32
#elif LECO_TARGET_ANDROID
#pragma leco add_impl android
#pragma leco add_library android
#elif LECO_TARGET_LINUX
#pragma leco add_impl linux
#pragma leco add_library X11
#endif
