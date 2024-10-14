#pragma once
#include "common.hpp"

extern "C" void casein_clear_drops();
extern "C" void casein_add_drop(const char * name, unsigned cch);

extern "C" void casein_call(casein::event_type et);
extern "C" void casein_call_g(casein::event_type et, casein::gestures g);
extern "C" void casein_call_k(casein::event_type et, casein::keys k);
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons m);
extern "C" float * casein_screen_scale_factor;
extern "C" casein::point * casein_mouse_pos;
extern "C" casein::point * casein_mouse_rel;
extern "C" casein::point * casein_window_size;
extern "C" bool * casein_fullscreen;
extern "C" bool * casein_window_live_resize;

extern "C" void casein_window_title(const char ** ptr, unsigned * len);
