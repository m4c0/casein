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

#pragma leco add_impl externc
#pragma leco add_impl handler

#ifdef LECO_TARGET_MACOSX
#pragma leco add_impl CASView osx apple
#pragma leco add_framework AppKit MetalKit Metal
#elif LECO_TARGET_IOS
#pragma leco add_impl CASView ios apple
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
