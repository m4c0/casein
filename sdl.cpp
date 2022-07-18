#include "casein.hpp"

#include <SDL.h>
#include <memory>
#include <stdexcept>

static auto create_window() {
  SDL_Window * wnd = SDL_CreateWindow("App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
  if (!wnd) {
    throw std::runtime_error("Failed to create SDL window");
  }
  return std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> { wnd, &SDL_DestroyWindow };
}

static auto create_renderer(SDL_Window * wnd) {
  SDL_Renderer * rnd = SDL_CreateRenderer(wnd, -1, 0);
  if (!rnd) {
    throw std::runtime_error("Failed to create SDL renderer");
  }
  return std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> { rnd, &SDL_DestroyRenderer };
}

int main(int /**/, char ** /**/) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Failed to initialise SDL");
  }

  auto wnd = create_window();
  auto rnd = create_renderer(wnd.get());

  casein_event(casein::events::create_window { rnd.get() });
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        casein_event(casein::events::key_down { 0 });
        break;
      case SDL_KEYUP:
        casein_event(casein::events::key_up { 0 });
        break;
      case SDL_QUIT:
        casein_event(casein::events::quit {});
        SDL_Quit();
        return 0;
      default:
        break;
      }
    }

    SDL_SetRenderDrawColor(rnd.get(), 0, 0, 0, 255);
    SDL_RenderClear(rnd.get());
    casein_event(casein::events::repaint {});
    SDL_RenderPresent(rnd.get());
  }
}
