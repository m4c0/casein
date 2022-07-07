#include "casein.hpp"
#include "casein.sdl.hpp"

#include <SDL.h>
#include <memory>
#include <stdexcept>

[[nodiscard]] static auto & renderer() {
  static std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> rnd { nullptr, &SDL_DestroyRenderer };
  return rnd;
}

static void init_renderer(void * handle) {
  static std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> wnd { nullptr, &SDL_DestroyWindow };

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Failed to initialise SDL");
  }

  wnd.reset(SDL_CreateWindowFrom(handle));
  if (!wnd) {
    throw std::runtime_error("Failed to create SDL window");
  }

  auto & rnd = renderer();
  rnd.reset(SDL_CreateRenderer(wnd.get(), -1, 0));
  if (!rnd) {
    throw std::runtime_error("Failed to create SDL renderer");
  }
}

void casein_event(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    init_renderer(e.as<casein::events::create_window>().native_window_handle());
    // Dispatch a new event, but using SDL_Renderer as the new native handle
    casein_sdl_event(casein::events::create_window { renderer().get() });
    break;
  case casein::QUIT:
    renderer().reset();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    casein_sdl_event(e);
    break;
  default:
    casein_sdl_event(e);
    break;
  }
}
