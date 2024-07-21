module casein;

extern "C" void casein_set_title(const char *);
void casein::set_title(const char * title) {
  casein_set_title(title);
}
