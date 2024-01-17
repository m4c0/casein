export module casein;
export import :events;
export import :handler;

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
#elif LECO_TARGET_WASM
#pragma leco add_impl externc wasm
#endif
