module;
#pragma leco add_library X11
#include <X11/Xlib.h>

struct casein_native_handle {
  Display * display;
  Window window;
};

module casein;
import silog;

static volatile bool should_quit = false;
static volatile int exit_code = 0;

static casein_native_handle nptr {};

extern "C" void casein_exit(int code) {
  exit_code = code;
  should_quit = true;
}
extern "C" void casein_set_title(const char * title) {
  XStoreName(nptr.display, nptr.window, title);
}

extern "C" int main() {
  auto dpy = XOpenDisplay(nullptr);
  if (!dpy) {
    silog::log(silog::error, "ERROR: could not open display");
    return 1;
  }

  auto scr = DefaultScreen(dpy);
  auto rootwin = RootWindow(dpy, scr);

  auto win = XCreateSimpleWindow(dpy, rootwin, 1, 1, 800, 450, 0, BlackPixel(dpy, scr), BlackPixel(dpy, scr));
  XStoreName(dpy, win, "app");

  XSelectInput(dpy, win, ExposureMask | ButtonPressMask);

  XMapWindow(dpy, win);

  nptr = {
    .display = dpy,
    .window = win,
  };
  casein::native_ptr = &nptr;
  casein_call(casein::CREATE_WINDOW);

  while (!should_quit) {
    XEvent e {};
    XNextEvent(dpy, &e);
    // if (e.type == ButtonPress) break;
  }

  casein_call(casein::QUIT);
  XCloseDisplay(dpy);
  return exit_code;
}
