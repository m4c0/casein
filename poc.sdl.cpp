#include "casein.hpp"

#include <SDL_render.h>
#include <SDL_timer.h>

static int x = 0;
static int y = 0;
static bool down = false;

static void draw_frame(SDL_Renderer * r) {
  SDL_SetRenderDrawColor(r, x % 256, y % 256, down ? 255 : 0, 255); // NOLINT
  SDL_RenderFillRect(r, nullptr);
}

void casein_event(const casein::event & e) {
  static SDL_Renderer * renderer;

  switch (e.type()) {
  case casein::CREATE_WINDOW:
    renderer = static_cast<SDL_Renderer *>(e.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::MOUSE_DOWN:
    down = true;
    break;
  case casein::MOUSE_MOVE:
    x = e.as<casein::events::mouse_move>().at().x;
    y = e.as<casein::events::mouse_move>().at().y;
    break;
  case casein::MOUSE_UP:
    down = false;
    break;
  case casein::REPAINT:
    draw_frame(renderer);
    break;
  default:
    break;
  }
}
