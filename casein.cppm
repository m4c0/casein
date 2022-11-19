export module casein;

export namespace casein {
  enum event_type {
    CREATE_WINDOW,
    KEY_DOWN,
    KEY_UP,
    MOUSE_DOWN,
    MOUSE_MOVE,
    MOUSE_UP,
    REPAINT,
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

  protected:
    [[nodiscard]] constexpr A argument() const noexcept {
      return a;
    }

  public:
    constexpr single_arg_event(A a) : event(ET), a(a) {
    }
  };

  template<event_type ET>
  struct key_event : public single_arg_event<ET, int> {
    using single_arg_event<ET, int>::single_arg_event;

    [[nodiscard]] constexpr int key_code() const noexcept {
      return single_arg_event<ET, int>::argument();
    }
  };

  template<event_type ET>
  struct mouse_button_event : public single_arg_event<ET, int> {
    using single_arg_event<ET, int>::single_arg_event;

    [[nodiscard]] constexpr int button() const noexcept {
      return single_arg_event<ET, int>::argument();
    }
  };

  struct create_window : public single_arg_event<CREATE_WINDOW, void *> {
    using single_arg_event::single_arg_event;

    [[nodiscard]] constexpr void * native_window_handle() const noexcept {
      return argument();
    }
  };
  struct mouse_move : public single_arg_event<MOUSE_MOVE, point> {
    constexpr mouse_move(int x, int y) : single_arg_event(point { x, y }) {
    }

    [[nodiscard]] constexpr point at() const noexcept {
      return argument();
    }
  };
  using key_down = key_event<KEY_DOWN>;
  using key_up = key_event<KEY_UP>;
  using mouse_down = mouse_button_event<MOUSE_DOWN>;
  using mouse_up = mouse_button_event<MOUSE_UP>;
  using repaint = empty_event<REPAINT>;
  using quit = empty_event<QUIT>;
}
export namespace casein {
  class event_handler;
  event_handler *& handler() {
    static event_handler * i;
    return i;
  }

  struct event_handler {
    event_handler() {
      handler() = this;
    }
    virtual void handle(const casein::event & e) = 0;
  };
}
