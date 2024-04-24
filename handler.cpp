module casein;
import :call;
import silog;

extern "C" casein::native_handle_t * casein_native_ptr = &casein::native_ptr;

namespace casein {
  using fn_t = void (*)();
  static fn_t emap[MAX_EVENT_TYPE] {};
  static fn_t emap_g[MAX_EVENT_TYPE][G_MAX] {};
  static fn_t emap_k[MAX_EVENT_TYPE][K_MAX] {};
  static fn_t emap_m[MAX_EVENT_TYPE][M_MAX] {};

  void handle(event_type et, void (*fn)()) {
    emap[et] = fn;
  }
  void handle(event_type et, keys k, void (*fn)()) {
    emap_k[et][k] = fn;
  }
  void handle(event_type et, mouse_buttons m, void (*fn)()) {
    emap_m[et][m] = fn;
  }
  void handle(event_type et, gestures g, void (*fn)()) {
    emap_g[et][g] = fn;
  }

  void reset_k(event_type et) {
    for (auto & n : emap_k[et])
      n = {};
  }
  void reset_m(event_type et) {
    for (auto & n : emap_m[et])
      n = {};
  }
  void reset_g(event_type et) {
    for (auto & n : emap_g[et])
      n = {};
  }

  extern "C" void casein_handle(const event & e) {
    switch (e.type()) {
    case KEY_DOWN:
    case KEY_UP:
      call_k(e.type(), *e.as<events::key_down>());
      break;
    case GESTURE:
      call_g(e.type(), *e.as<events::gesture>());
      break;
    case MOUSE_DOWN:
    case MOUSE_UP:
      call_m(e.type(), *e.as<events::mouse_down>());
      break;
    default:
      call(e.type());
      break;
    }
  }
}

void casein::call(event_type et) {
  if (auto fn = emap[et]) {
    fn();
  }
}
void casein::call_g(event_type et, gestures g) {
  if (auto fn = emap_g[et][g]) {
    fn();
  }
  call(et);
}
void casein::call_k(event_type et, keys k) {
  if (auto fn = emap_k[et][k]) {
    fn();
  }
  call(et);
}
void casein::call_m(event_type et, mouse_buttons m) {
  if (auto fn = emap_m[et][m]) {
    fn();
  }
  call(et);
}
