#pragma leco app
import casein;
import silog;

static void on_window_created() {
  silog::log(silog::info, "window created");
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
  silog::log(silog::info, "mouse down");
}
static void resize_window() {
  silog::log(
      silog::info,
      "resize window - %dx%d @%.0fx",
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
  casein::set_fullscreen(!casein::is_fullscreen());
}

static void title() {
  static char buf[1024] = "title 0";
  if (++buf[6] > '9') {
    buf[6] = '0';
  }
  casein::set_title(buf);
}

static struct init {
  init() {
    using namespace casein;
    handle(CREATE_WINDOW, &on_window_created);
    handle(GESTURE, G_TAP_1, &tap_1);
    handle(GESTURE, G_TAP_2, &tap_2);
    handle(GESTURE, &gesture);
    handle(KEY_DOWN, K_LEFT, &left);
    handle(KEY_DOWN, K_RIGHT, &right);
    handle(KEY_DOWN, K_F, &flip_fullscreen);
    handle(KEY_DOWN, K_X, &title);
    handle(MOUSE_DOWN, &mouse_down);
    handle(RESIZE_WINDOW, &resize_window);
    handle(TOUCH_DOWN, &touch_down);
    handle(QUIT, &quit);
  }
} i;
