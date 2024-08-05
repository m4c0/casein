module;

#define EXPORT(X) __attribute__((export_name(#X))) X

extern "C" __attribute__((import_module("leco"), import_name("set_timeout"))) void set_timeout(void (*)(), unsigned);
extern "C" __attribute__((import_module("leco"), import_name("request_animation_frame"))) void request_animation_frame(
    void (*)());

module casein;
import silog;

extern "C" void casein_call(casein::event_type et);
extern "C" void casein_call_k(casein::event_type et, casein::keys);

extern "C" void casein_enable_filedrop(bool en) {
}

void casein::interrupt(casein::interrupts irq) {
  switch (irq) {
  case IRQ_FULLSCREEN:
    silog::log(silog::warning, "Fullscreen not supported");
    break;
  case IRQ_QUIT:
    silog::log(silog::warning, "Quit not supported");
    break;
  case IRQ_WINDOW_SIZE:
    silog::log(silog::warning, "Changing window size TBD");
    break;
  case IRQ_WINDOW_TITLE:
    silog::log(silog::warning, "Changing window title TBD");
    break;
  }
}

static void repaint() {
  casein_call(casein::REPAINT);
  request_animation_frame(repaint);
}

static constexpr auto key_for_code(int code) {
  switch (code) {
  case 32:
    return casein::K_SPACE;
  case 37:
    return casein::K_LEFT;
  case 38:
    return casein::K_UP;
  case 39:
    return casein::K_RIGHT;
  case 40:
    return casein::K_DOWN;
  default:
    return casein::K_NULL;
  }
}
void EXPORT(casein_key)(bool down, unsigned key_code) {
  auto key = key_for_code(key_code);
  auto code = down ? casein::KEY_DOWN : casein::KEY_UP;
  casein_call_k(code, key);
}

int main() {
  casein_call(casein::CREATE_WINDOW);
  request_animation_frame(repaint);
}
