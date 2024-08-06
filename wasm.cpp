module;

#define EXPORT(X) __attribute__((export_name(#X))) X
#define IMPORT(R, X) extern "C" __attribute__((import_module("casein"), import_name(#X))) R X

module casein;
import silog;
import vaselin;

IMPORT(void, window_size)(int x, int y);
IMPORT(void, window_title)(const char *, int);

// TODO: more keyboard keys
// TODO: mouse support
// TODO: centralise canvas (maybe a border too?)

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
    ::window_size(casein::window_size.x, casein::window_size.y);
    break;
  case IRQ_WINDOW_TITLE:
    ::window_title(casein::window_title.begin(), casein::window_title.size());
    break;
  }
}

static void repaint() {
  casein_call(casein::REPAINT);
  vaselin::request_animation_frame(repaint);
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
  casein::interrupt(casein::IRQ_WINDOW_SIZE);
  casein::interrupt(casein::IRQ_WINDOW_TITLE);

  casein_call(casein::CREATE_WINDOW);
  vaselin::request_animation_frame(repaint);
}
