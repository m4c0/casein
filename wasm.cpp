module;

extern "C" __attribute__((import_module("leco"), import_name("set_timeout"))) void set_timeout(void (*)(), unsigned);

module casein;
import silog;

extern "C" void casein_call(casein::event_type et);

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

struct init {
  init();
} i;
init::init() {
  set_timeout(
      [] {
        casein_call(casein::CREATE_WINDOW);
      },
      0);
}
