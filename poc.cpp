import casein;

extern "C" int printf(const char *, ...);

void on_window_created(auto /*handle*/) {
  // If this was SDL, you could do SDL_Init+SDL_CreateWindowFrom using the native handle provided by the argument of
  // the event. Vulkan can use it to initialise all its shenanigans, etc.
}

extern "C" void casein_handle(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    // You can fetch the native handle (HWND, NSWindow, etc) like this:
    on_window_created(*e.as<casein::events::create_window>());
    break;
  case casein::MOUSE_DOWN: {
    const auto & [x, y, btn] = *e.as<casein::events::mouse_down>();
    printf("mouse down: %d,%d -- button: %d\n", x, y, btn);
    break;
  }
  case casein::RESIZE_WINDOW: {
    const auto & [w, h, live] = *e.as<casein::events::resize_window>();
    printf("resizing: %d,%d -- live: %d\n", w, h, live);
    break;
  }
  case casein::QUIT:
    // SDL_Quit, release shenanigans, etc
    break;
  default:
    break;
  }
}
