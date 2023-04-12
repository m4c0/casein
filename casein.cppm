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

export namespace casein {
  using native_handle_t = ::casein_native_handle *;

#include "keys.hpp"

  enum event_type {
    CREATE_WINDOW,
    GESTURE,
    KEY_DOWN,
    KEY_UP,
    MOUSE_DOWN,
    MOUSE_MOVE,
    MOUSE_UP,
    REPAINT,
    RESIZE_WINDOW,
    QUIT,
  };

  class event;

  template<typename T>
  concept is_event = requires(const T * t) { static_cast<const event *>(t); };

  class event {
    event_type t;

  protected:
    constexpr event(event_type t) noexcept : t { t } {
    }

  public:
    [[nodiscard]] constexpr event_type type() const noexcept {
      return t;
    }

    template<is_event Tp>
    [[nodiscard]] constexpr const Tp & as() const noexcept {
      return static_cast<const Tp &>(*this);
    }
  };

  struct point {
    int x;
    int y;
  };
  struct click {
    int x;
    int y;
    int button;
  };
  struct resize {
    int width;
    int height;
    float scale_factor;
    bool live;
  };
}
export namespace casein::events {
  template<event_type ET>
  class empty_event : public event {
  public:
    constexpr empty_event() : event(ET) {
    }
  };

  template<event_type ET, typename A>
  class single_arg_event : public event {
    A a;

  public:
    constexpr single_arg_event(A a) : event(ET), a(a) {
    }

    [[nodiscard]] constexpr A operator*() const noexcept {
      return a;
    }
  };

  using create_window = single_arg_event<CREATE_WINDOW, native_handle_t>;
  using gesture = single_arg_event<GESTURE, gestures>;
  using key_down = single_arg_event<KEY_DOWN, keys>;
  using key_up = single_arg_event<KEY_UP, keys>;
  using mouse_down = single_arg_event<MOUSE_DOWN, click>;
  using mouse_move = single_arg_event<MOUSE_MOVE, point>;
  using mouse_up = single_arg_event<MOUSE_UP, click>;
  using quit = empty_event<QUIT>;
  using repaint = empty_event<REPAINT>;
  using resize_window = single_arg_event<RESIZE_WINDOW, resize>;
}
extern "C" void casein_handle(const casein::event & e);
