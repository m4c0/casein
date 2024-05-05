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

export {
#include "common.hpp"
}

export namespace casein {
  extern point mouse_pos;
  extern point mouse_rel;
  extern bool keydown_repeating;

  void set_title(const char * title);

  // Signal the app to eventually exit.
  void exit(int code);

  bool is_fullscreen();
  void set_fullscreen(bool);

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

casein::native_handle_t casein::native_ptr;
casein::point casein::mouse_pos;
casein::point casein::mouse_rel;
bool casein::keydown_repeating;

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
