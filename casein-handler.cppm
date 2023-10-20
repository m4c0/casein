export module casein:handler;
import :events;

export namespace casein {
  struct handler {
    virtual ~handler() = default;

    virtual void create_window(const events::create_window &) {
    }
    virtual void gesture(const events::gesture &) {
    }
    virtual void key_down(const events::key_down &) {
    }
    virtual void key_up(const events::key_up &) {
    }
    virtual void mouse_down(const events::mouse_down &) {
    }
    virtual void mouse_move(const events::mouse_move &) {
    }
    virtual void mouse_up(const events::mouse_up &) {
    }
    virtual void repaint(const events::repaint &) {
    }
    virtual void resize_window(const events::resize_window &) {
    }
    virtual void quit(const events::quit &) {
    }
    virtual void timer(const events::timer &) {
    }
    virtual void touch_cancel(const events::touch_cancel &) {
    }
    virtual void touch_down(const events::touch_down &) {
    }
    virtual void touch_move(const events::touch_move &) {
    }
    virtual void touch_up(const events::touch_up &) {
    }

    void handle(const event & e) {
      switch (e.type()) {
      case CREATE_WINDOW:
        create_window(e.as<events::create_window>());
        break;
      case GESTURE:
        gesture(e.as<events::gesture>());
        break;
      case KEY_DOWN:
        key_down(e.as<events::key_down>());
        break;
      case KEY_UP:
        key_up(e.as<events::key_up>());
        break;
      case MOUSE_DOWN:
        mouse_down(e.as<events::mouse_down>());
        break;
      case MOUSE_MOVE:
        mouse_move(e.as<events::mouse_move>());
        break;
      case MOUSE_UP:
        mouse_up(e.as<events::mouse_up>());
        break;
      case REPAINT:
        repaint(e.as<events::repaint>());
        break;
      case RESIZE_WINDOW:
        resize_window(e.as<casein::events::resize_window>());
        break;
      case QUIT:
        quit(e.as<events::quit>());
        break;
      case TIMER:
        timer(e.as<events::timer>());
        break;
      case TOUCH_CANCEL:
        touch_cancel(e.as<events::touch_cancel>());
        break;
      case TOUCH_DOWN:
        touch_down(e.as<events::touch_down>());
        break;
      case TOUCH_MOVE:
        touch_move(e.as<events::touch_move>());
        break;
      case TOUCH_UP:
        touch_up(e.as<events::touch_up>());
        break;
      case MAX_EVENT_TYPE:
        break;
      }
    }
  };
} // namespace casein
