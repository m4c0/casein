module;

extern "C" __attribute__((import_module("leco"), import_name("set_timeout"))) void set_timeout(void (*)(), unsigned);

module casein;
import silog;

extern "C" void casein_enable_filedrop(bool en) {
}

void casein::interrupt(casein::interrupts irq) {
}

int main() {
  set_timeout(
      [] {
        silog::log(silog::debug, "ohh");
      },
      0);
}
