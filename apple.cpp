module casein;

extern "C" void casein_interrupt(casein::interrupts irq);
void casein::interrupt(casein::interrupts irq) {
  casein_interrupt(irq);
}

extern "C" void casein_exit(int);
void casein::exit(int code) {
  casein_exit(code);
}
