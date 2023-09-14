module;

#ifdef __APPLE__
typedef struct CAMetalLayer casein_native_handle;
#elif defined(_WIN32)
typedef struct HWND__ casein_native_handle;
#elif defined(__ANDROID__)
typedef struct ANativeWindow casein_native_handle;
#elif defined(__wasm__)
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

  template<typename ET, ET::type Max>
  class subevent_map {
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

  using key_map = subevent_map<events::key_down, K_MAX>;
  using gesture_map = subevent_map<events::gesture, G_MAX>;
}

#ifdef LECO_TARGET_MACOSX
#pragma leco add_impl CASView osx
#pragma leco add_framework AppKit MetalKit Metal
#elif LECO_TARGET_IOS
#pragma leco add_impl CASView ios
#pragma leco add_framework MetalKit UIKit
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#pragma leco add_library user32
#elif LECO_TARGET_ANDROID
#pragma leco add_impl android
#pragma leco add_library android
#elif LECO_TARGET_WASM
#pragma leco add_impl externc wasm
#endif
