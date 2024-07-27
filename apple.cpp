module casein;

extern "C" void casein_interrupt(casein::interrupts irq);
void casein::interrupt(casein::interrupts irq) {
  casein_interrupt(irq);
}
