#pragma once

namespace casein {
  using native_handle_t = ::casein_native_handle *;

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
    TOUCH_CANCEL,
    TOUCH_DOWN,
    TOUCH_MOVE,
    TOUCH_UP,
  };
  enum keys : unsigned {
    K_NULL,
    K_SPACE = ' ',
    K_DOWN = 0x100,
    K_LEFT,
    K_RIGHT,
    K_UP,
  };
  enum gestures {
    G_LONG_PRESS,
    G_TAP_1,
    G_TAP_2,
    G_SHAKE,
    G_SWIPE_DOWN,
    G_SWIPE_LEFT,
    G_SWIPE_RIGHT,
    G_SWIPE_UP,
  };

  class event;

  class event {
    event_type t;

  protected:
    constexpr event(event_type t) noexcept : t { t } {
    }

  public:
    [[nodiscard]] constexpr event_type type() const noexcept {
      return t;
    }

    template<typename Tp>
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

namespace casein::events {
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
  using touch_cancel = single_arg_event<TOUCH_CANCEL, point>;
  using touch_down = single_arg_event<TOUCH_DOWN, point>;
  using touch_move = single_arg_event<TOUCH_MOVE, point>;
  using touch_up = single_arg_event<TOUCH_UP, point>;
}

extern "C" void casein_handle(const casein::event & e);
