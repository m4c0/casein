#pragma leco app
import casein;
import silog;

void on_window_created(auto /*handle*/) {
  // If this was SDL, you could do SDL_Init+SDL_CreateWindowFrom using the native handle provided by the argument of
  // the event. Vulkan can use it to initialise all its shenanigans, etc.
}

struct me_handler : public casein::handler {
  void mouse_down() override {
    silog::log(silog::info, "mouse down");
  }
};

extern "C" void casein_handle(const casein::event & e) {
  static me_handler h {};
  h.handle(e);

  switch (e.type()) {
  case casein::CREATE_WINDOW:
    // You can fetch the native handle (HWND, NSWindow, etc) like this:
    on_window_created(*e.as<casein::events::create_window>());
    silog::log(silog::info, "window created");
    break;
  case casein::GESTURE:
    switch (*e.as<casein::events::gesture>()) {
    case casein::G_TAP_1:
      silog::log(silog::info, "tap");
      break;
    case casein::G_TAP_2:
      silog::log(silog::info, "double tap");
      break;
    default:
      silog::log(silog::info, "gesture");
      break;
    }
    break;
  case casein::RESIZE_WINDOW: {
    [[maybe_unused]] const auto & [w, h, scale, live] = *e.as<casein::events::resize_window>();
    silog::log(silog::info, "resize window");
    break;
  }
  case casein::QUIT:
    // SDL_Quit, release shenanigans, etc
    silog::log(silog::info, "quit");
    break;
  case casein::TOUCH_DOWN: {
    const auto & [x, y, lp] = *e.as<casein::events::touch_down>();
    silog::log(silog::info, lp ? "long press down" : "touch down");
    break;
  }
  default:
    break;
  }
}
