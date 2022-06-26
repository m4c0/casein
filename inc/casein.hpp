#pragma once

namespace casein {
  enum event_type { START, REPAINT, STOP, QUIT };

  class event {
    event_type t;

  protected:
    constexpr event(event_type t) noexcept : t { t } {
    }

  public:
    [[nodiscard]] constexpr event_type type() const noexcept {
      return t;
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

  using start = empty_event<START>;
  using repaint = empty_event<REPAINT>;
  using stop = empty_event<STOP>;
  using quit = empty_event<QUIT>;
}
void casein_event(const casein::event & e);
