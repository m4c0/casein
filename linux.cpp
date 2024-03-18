#include <X11/Xlib.h>

import silog;

extern "C" void casein_exit(int code) {
}
int main() {
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

  while (true) {
    XEvent e{};
    XNextEvent(dpy, &e);
    if (e.type == ButtonPress) break;
  }

  XCloseDisplay(dpy);
}
