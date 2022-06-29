#include "casein.sdl.hpp"

#include <SDL_render.h>
#include <SDL_timer.h>

static void draw_frame(SDL_Renderer * r) {
  const auto y = SDL_GetTicks() % 256;
  SDL_SetRenderDrawColor(r, 255, y, 0, 255);
  SDL_RenderClear(r);
  SDL_RenderPresent(r);
}

void casein_sdl_event(const casein::event & e) {
  static SDL_Renderer * renderer;

  switch (e.type()) {
  case casein::CREATE_WINDOW:
    renderer = static_cast<SDL_Renderer *>(e.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::REPAINT:
    draw_frame(renderer);
    break;
  default:
    break;
  }
}
