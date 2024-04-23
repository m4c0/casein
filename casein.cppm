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
using casein_native_handle = void;
#endif

export module casein;

export {
#include "common.hpp"
}
export namespace casein {
  class event_map {
    using callback = void (*)(const event & e);

    callback m_map[MAX_EVENT_TYPE];

  public:
    [[nodiscard]] constexpr auto & operator[](event_type idx) noexcept {
      return m_map[idx];
    }

    constexpr void handle(const event & e) const {
      auto fn = m_map[e.type()];
      if (fn) fn(e);
    }
  };

  template<typename ET, ET::type Max, typename Handler>
  class subevent_map {
    using callback = void (Handler::*)();

    callback m_map[Max] {};
    Handler * m_handler;

  public:
    explicit constexpr subevent_map(Handler * h) noexcept : m_handler { h } {
    }

    [[nodiscard]] constexpr auto & operator[](ET::type idx) noexcept {
      return m_map[idx];
    }

    constexpr void handle(const event & e) const {
      auto fn = m_map[*e.as<ET>()];
      if (fn) (m_handler->*fn)();
    }
  };
  template<typename ET, ET::type Max>
  class subevent_map<ET, Max, void> {
    using callback = void (*)(const event & e);

    callback m_map[Max];

  public:
    [[nodiscard]] constexpr auto & operator[](ET::type idx) noexcept {
      return m_map[idx];
    }

    constexpr void handle(const event & e) const {
      auto fn = m_map[*e.as<ET>()];
      if (fn) fn(e);
    }
  };

  template<typename Handler = void>
  struct key_down_map : subevent_map<events::key_down, K_MAX, Handler> {
    using subevent_map<events::key_down, K_MAX, Handler>::subevent_map;
  };
  template<typename H>
  key_down_map(H *) -> key_down_map<H>;

  template<typename Handler = void>
  struct gesture_map : subevent_map<events::gesture, G_MAX, Handler> {
    using subevent_map<events::gesture, G_MAX, Handler>::subevent_map;
  };
  template<typename H>
  gesture_map(H *) -> gesture_map<H>;
} // namespace casein

extern "C" void casein_exit(int);
namespace casein {
  // Signal the app to eventually exit.
  export void exit(int code) {
    casein_exit(code);
  }

  void call(event_type et);
  void call_g(event_type et, gestures g);
  void call_k(event_type et, keys k);
  void call_m(event_type et, mouse_buttons m);
}

export namespace casein {
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

casein::native_handle_t casein::native_ptr;

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
