export module casein:handler;
import :events;

export namespace casein {
  struct handler {
    virtual ~handler() = default;

    virtual void create_window(const events::create_window &) {
    }
    virtual void gesture() {
    }
    virtual void key_down() {
    }
    virtual void key_up() {
    }
    virtual void mouse_down() {
    }
    virtual void mouse_move() {
    }
    virtual void mouse_up() {
    }
    virtual void repaint() {
    }
    virtual void resize_window(const events::resize_window &) {
    }
    virtual void quit() {
    }
    virtual void timer() {
    }
    virtual void touch_cancel() {
    }
    virtual void touch_down() {
    }
    virtual void touch_move() {
    }
    virtual void touch_up() {
    }

    void handle(const event & e) {
      switch (e.type()) {
      case CREATE_WINDOW:
        create_window(e.as<events::create_window>());
        break;
      case GESTURE:
        gesture();
        break;
      case KEY_DOWN:
        key_down();
        break;
      case KEY_UP:
        key_up();
        break;
      case MOUSE_DOWN:
        mouse_down();
        break;
      case MOUSE_MOVE:
        mouse_move();
        break;
      case MOUSE_UP:
        mouse_up();
        break;
      case REPAINT:
        repaint();
        break;
      case RESIZE_WINDOW:
        resize_window(e.as<casein::events::resize_window>());
        break;
      case QUIT:
        quit();
        break;
      case TIMER:
        timer();
        break;
      case TOUCH_CANCEL:
        touch_cancel();
        break;
      case TOUCH_DOWN:
        touch_down();
        break;
      case TOUCH_MOVE:
        touch_move();
        break;
      case TOUCH_UP:
        touch_up();
        break;
      case MAX_EVENT_TYPE:
        break;
      }
    }
  };
} // namespace casein
