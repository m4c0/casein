module casein;

extern "C" void casein_enter_fullscreen();
void casein::enter_fullscreen() {
  casein_enter_fullscreen();
}

extern "C" void casein_exit(int);
void casein::exit(int code) {
  casein_exit(code);
}

extern "C" void casein_leave_fullscreen();
void casein::leave_fullscreen() {
  casein_leave_fullscreen();
}

extern "C" void casein_set_title(const char *);
void casein::set_title(const char * title) {
  casein_set_title(title);
}
