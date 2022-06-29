#pragma once

namespace casein {
  enum event_type { CREATE_WINDOW, REPAINT, QUIT };

  class event;

  template<typename T>
  concept is_event = requires(const T * t) {
    static_cast<const event *>(t);
  };

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
}
namespace casein::events {
  template<event_type ET>
  class empty_event : public event {
  public:
    constexpr empty_event() : event(ET) {
    }
  };
  using repaint = empty_event<REPAINT>;
  using quit = empty_event<QUIT>;

  template<event_type ET, typename A>
  class single_arg_event : public event {
    A a;

  protected:
    [[nodiscard]] constexpr A argument() const noexcept {
      return a;
    }

  public:
    constexpr single_arg_event(A a) : event(ET), a(a) {
    }
  };
  struct create_window : public single_arg_event<CREATE_WINDOW, void *> {
    using single_arg_event::single_arg_event;

    [[nodiscard]] constexpr void * native_window_handle() const noexcept {
      return argument();
    }
  };
}
void casein_event(const casein::event & e);
