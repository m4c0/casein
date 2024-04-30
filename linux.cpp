module;
#pragma leco add_library X11
#include <X11/Xlib.h>

struct casein_native_handle {
  Display *display;
  Window window;
};

export module casein:linux;

import :events;
import silog;

static volatile bool should_quit = false;
static volatile int exit_code = 0;

extern "C" void casein_handle(const casein::event &);
extern "C" void casein_exit(int code) {
  exit_code = code;
  should_quit = true;
}
extern "C" int main() {
  auto dpy = XOpenDisplay(nullptr);
  if (!dpy) {
    silog::log(silog::error, "ERROR: could not open display");
    return 1;
  }

  auto scr = DefaultScreen(dpy);
  auto rootwin = RootWindow(dpy, scr);

  auto win = XCreateSimpleWindow(dpy, rootwin, 1, 1, 1280, 720, 0, BlackPixel(dpy, scr), BlackPixel(dpy, scr));
  XStoreName(dpy, win, "app");

  XSelectInput(dpy, win, ExposureMask | ButtonPressMask);

  XMapWindow(dpy, win);

  casein_native_handle nptr {
    .display = dpy,
    .window = win,
  };
  casein_handle(casein::events::create_window { &nptr });

  while (!should_quit) {
    XEvent e {};
    XNextEvent(dpy, &e);
    if (e.type == ButtonPress) break;
  }

  XCloseDisplay(dpy);
  return exit_code;
}
