#pragma leco app
import casein;
import silog;

static void on_window_created() {
  silog::log(silog::info, "window created");
}
static void file_dropped() {
  for (const auto & f : casein::dropped_files) {
    silog::log(silog::info, "file dropped: %s", f.begin());
  }
}
static void gesture() {
  silog::log(silog::info, "gesture");
}
static void left() {
  silog::log(silog::info, "<left> key down");
}
static void right() {
  silog::log(silog::info, "<right> key down");
}
static void mouse_down() {
  silog::log(silog::info, "mouse down - %fx%f", casein::mouse_pos.x, casein::mouse_pos.y);
}
static void resize_window() {
  silog::log(
      silog::info,
      "resize window - %s %fx%f @%.0fx",
      casein::window_live_resize? "[L]" : "[ ]",
      casein::window_size.x,
      casein::window_size.y,
      casein::screen_scale_factor);
}
static void quit() {
  silog::log(silog::info, "quit");
}
static void tap_1() {
  silog::log(silog::info, "tap");
}
static void tap_2() {
  silog::log(silog::info, "double tap");
}
static void touch_down() {
  // TODO: read x/y/lp
  silog::log(silog::info, "touch down");
}
static void flip_fullscreen() {
  using namespace casein;
  fullscreen = !fullscreen;
  interrupt(IRQ_FULLSCREEN);
}

static void title() {
  static char buf[1024] = "title 0";
  if (++buf[6] > '9') {
    buf[6] = '0';
  }

  using namespace casein;
  window_title = buf;
  interrupt(IRQ_WINDOW_TITLE);
}
static void resize() {
  using namespace casein;

  window_size.x = window_size.x == 300 ? 600 : 300;
  window_size.y = window_size.y == 300 ? 600 : 300;
  interrupt(IRQ_WINDOW_SIZE);
}
static void exit() {
  using namespace casein;
  interrupt(IRQ_QUIT);
}

static struct init {
  init() {
    using namespace casein;

    handle(CREATE_WINDOW, &on_window_created);
    handle(FILES_DROP, &file_dropped);
    handle(GESTURE, G_TAP_1, &tap_1);
    handle(GESTURE, G_TAP_2, &tap_2);
    handle(GESTURE, &gesture);
    handle(KEY_DOWN, K_LEFT, &left);
    handle(KEY_DOWN, K_RIGHT, &right);
    handle(KEY_DOWN, K_F, &flip_fullscreen);
    handle(KEY_DOWN, K_Q, &exit);
    handle(KEY_DOWN, K_X, &title);
    handle(KEY_DOWN, K_Z, &resize);
    handle(KEY_DOWN, K_COMMA, &title);
    handle(KEY_DOWN, K_DOT, &title);
    handle(KEY_DOWN, K_SLASH, &title);
    handle(MOUSE_DOWN, &mouse_down);
    handle(RESIZE_WINDOW, &resize_window);
    handle(TOUCH_DOWN, &touch_down);
    handle(QUIT, &quit);

    window_title = "Casein Proof-of-Concept";
    window_size = { 300, 300 };
  }
} i;
