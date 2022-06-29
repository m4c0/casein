#include "casein.sdl.hpp"

#include <SDL_render.h>

static void draw_frame(SDL_Renderer * r) {
  SDL_RenderClear(r);
  SDL_RenderPresent(r);
}

void casein_sdl_event(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    // This is funky but works. We can trigger SDL_Render* commands right after
    // we create the window
    draw_frame(static_cast<SDL_Renderer *>(e.as<casein::events::create_window>().native_window_handle()));
    break;
  default:
    break;
  }
}
