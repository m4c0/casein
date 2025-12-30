module;
#pragma leco add_library X11
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>

// TODO: resize event

struct casein_native_handle {
  Display * display;
  Window window;
};

module casein;
import silog;

static volatile bool should_quit = false;
static volatile int exit_code = 0;

static casein_native_handle nptr {};

void casein::exit(int code) {
  exit_code = code;
  should_quit = true;
}
void casein::set_title(const char * title) {
  XStoreName(nptr.display, nptr.window, title);
}

void casein::enter_fullscreen() {
}
void casein::leave_fullscreen() {
}

static casein::keys key_of(XKeyEvent * ke) {
  switch (auto c = XLookupKeysym(ke, 0)) {
  case XK_Down:
    return casein::K_DOWN;
  case XK_Return:
    return casein::K_ENTER;
  case XK_Escape:
    return casein::K_ESCAPE;
  case XK_Left:
    return casein::K_LEFT;
  case XK_Right:
    return casein::K_RIGHT;
  case XK_Up:
    return casein::K_UP;
  default:
    if (c >= 32 && c <= 127) return static_cast<casein::keys>(c);
    return casein::K_NULL;
  }
}
static void key(casein::event_type evt, XEvent * e) {
  casein_call_k(evt, key_of(&e->xkey));
}

extern "C" void casein_init();
extern "C" int main() try {
  casein_init();

  auto dpy = XOpenDisplay(nullptr);
  if (!dpy) {
    silog::log(silog::error, "ERROR: could not open display");
    return 1;
  }

  auto scr = DefaultScreen(dpy);
  auto rootwin = RootWindow(dpy, scr);

  auto win = XCreateSimpleWindow(dpy, rootwin, 1, 1, 800, 450, 0, BlackPixel(dpy, scr), BlackPixel(dpy, scr));
  XStoreName(dpy, win, casein::window_title.cstr().begin());

  XSelectInput(dpy, win, ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);

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
    switch (e.type) {
    case Expose:
      casein_call(casein::RESIZE_WINDOW);
      break;
    case ButtonPress:
      break;
    case ButtonRelease:
      break;
    case KeyPress:
      key(casein::KEY_DOWN, &e);
      break;
    case KeyRelease:
      key(casein::KEY_UP, &e);
      break;
    }
  }

  casein_call(casein::QUIT);
  XCloseDisplay(dpy);
  return exit_code;
} catch (...) {
  silog::error("Unhandled error");
  return 1;
}
