import casein;

void on_window_created(void * /*handle*/) {
  // If this was SDL, you could do SDL_Init+SDL_CreateWindowFrom using the native handle provided by the argument of
  // the event. Vulkan can use it to initialise all its shenanigans, etc.
}

extern "C" void casein_handle(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    // You can fetch the native handle (HWND, NSWindow, etc) like this:
    on_window_created(e.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::QUIT:
    // SDL_Quit, release shenanigans, etc
    break;
  default:
    break;
  }
}
