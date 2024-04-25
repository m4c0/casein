#pragma once
#include "common.hpp"

namespace casein {
  class event;

  class event {
    event_type t;

  protected:
    inline constexpr event(event_type t) noexcept : t { t } {
    }

  public:
    [[nodiscard]] inline constexpr event_type type() const noexcept {
      return t;
    }

    template<typename Tp>
    [[nodiscard]] inline constexpr const Tp & as() const noexcept {
      return static_cast<const Tp &>(*this);
    }
  };

  struct point {
    int x;
    int y;
  };
  struct touch {
    int x;
    int y;
    bool long_press;
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
    using type = A;

    constexpr single_arg_event(A a) : event(ET), a(a) {
    }

    [[nodiscard]] constexpr A operator*() const noexcept {
      return a;
    }
  };

  using create_window = empty_event<CREATE_WINDOW>;
  using gesture = single_arg_event<GESTURE, gestures>;
  using key_down = single_arg_event<KEY_DOWN, keys>;
  using key_up = single_arg_event<KEY_UP, keys>;
  using mouse_down = single_arg_event<MOUSE_DOWN, mouse_buttons>;
  using mouse_move = single_arg_event<MOUSE_MOVE, point>;
  using mouse_move_rel = single_arg_event<MOUSE_MOVE_REL, point>;
  using mouse_up = single_arg_event<MOUSE_UP, mouse_buttons>;
  using quit = empty_event<QUIT>;
  using repaint = empty_event<REPAINT>;
  using resize_window = single_arg_event<RESIZE_WINDOW, resize>;
  using timer = empty_event<TIMER>;
  using touch_cancel = single_arg_event<TOUCH_CANCEL, touch>;
  using touch_down = single_arg_event<TOUCH_DOWN, touch>;
  using touch_move = single_arg_event<TOUCH_MOVE, touch>;
  using touch_up = single_arg_event<TOUCH_UP, touch>;
}

extern "C" void casein_handle(const casein::event & e);
