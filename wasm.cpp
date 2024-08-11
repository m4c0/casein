module;

#define EXPORT(X) __attribute__((export_name(#X))) X
#define IMPORT(R, X) extern "C" __attribute__((import_module("casein"), import_name(#X))) R X

module casein;
import silog;
import vaselin;

namespace {
  enum js_mouse_event { jsme_down = 0, jsme_move, jsme_up };
}

IMPORT(void, window_size)(int x, int y);
IMPORT(void, window_title)(const char *, int);

// TODO: more keyboard keys

// TODO: centralise these in "externc"
extern "C" void casein_call(casein::event_type et);
extern "C" void casein_call_k(casein::event_type et, casein::keys);
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons);

extern "C" void casein_enable_filedrop(bool en) {}

void casein::interrupt(casein::interrupts irq) {
  switch (irq) {
    case IRQ_FULLSCREEN: silog::log(silog::warning, "Fullscreen not supported"); break;
    case IRQ_QUIT: silog::log(silog::warning, "Quit not supported"); break;
    case IRQ_WINDOW_SIZE: ::window_size(casein::window_size.x, casein::window_size.y); break;
    case IRQ_WINDOW_TITLE: ::window_title(casein::window_title.begin(), casein::window_title.size()); break;
  }
}

static void repaint(void *) {
  casein_call(casein::REPAINT);
  vaselin::request_animation_frame(repaint, nullptr);
}

static constexpr auto key_for_code(int code) {
  switch (code) {
    case 13: return casein::K_ENTER;
    case 32: return casein::K_SPACE;
    case 37: return casein::K_LEFT;
    case 38: return casein::K_UP;
    case 39: return casein::K_RIGHT;
    case 40: return casein::K_DOWN;
    default:
      if (code >= 'A' && code <= 'Z') return static_cast<casein::keys>(code - 'A' + 'a');
      if (code >= '0' && code <= '9') return static_cast<casein::keys>(code);
      return casein::K_NULL;
  }
}
void EXPORT(casein_key)(bool down, unsigned key_code) {
  auto key = key_for_code(key_code);
  auto code = down ? casein::KEY_DOWN : casein::KEY_UP;
  casein_call_k(code, key);
}

static auto js_button(int button) {
  switch (button) {
    case 0: return casein::M_LEFT;
    case 2: return casein::M_RIGHT;
    default: return casein::M_MAX;
  }
}
void EXPORT(casein_mouse)(int e, int button, int ofsx, int ofsy) {
  auto b = js_button(button);
  if (b == casein::M_MAX) return;

  casein::mouse_pos = { ofsx, ofsy };
  switch (e) {
    case jsme_down: casein_call_m(casein::MOUSE_DOWN, b);
    case jsme_move: casein_call(casein::MOUSE_MOVE);
    case jsme_up: casein_call_m(casein::MOUSE_UP, b);
  }
}

static void timer(void *) {
  casein_call(casein::TIMER);
  vaselin::set_timeout(timer, nullptr, 50);
}

int main() {
  casein::interrupt(casein::IRQ_WINDOW_SIZE);
  casein::interrupt(casein::IRQ_WINDOW_TITLE);

  casein_call(casein::CREATE_WINDOW);
  vaselin::request_animation_frame(repaint, nullptr);
  vaselin::set_timeout(timer, nullptr, 0);
}
