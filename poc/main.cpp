#include "casein.hpp"

void casein_event(const casein::event & e) {
  // Idea for usage - a global/static unique_ptr
  // static std::unique_ptr<my_stuff> s;
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    // If this was SDL, you could do SDL_Init+SDL_CreateWindowFrom using the native handle provided by the argument of
    // the event. Vulkan can use it to initialise all its shenanigans, etc.
    // s = std::make_unique<my_stuff>(e);
    break;
  case casein::QUIT:
    // s.reset();
    break;
  default:
    break;
  }
}
