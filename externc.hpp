#pragma once
#include "common.hpp"

inline constexpr const auto casein_base_width = 1280;
inline constexpr const auto casein_base_height = 720;

extern "C" void casein_clear_drops();
extern "C" void casein_add_drop(const char * name, unsigned cch);

extern "C" void casein_call(casein::event_type et);
extern "C" void casein_call_g(casein::event_type et, casein::gestures g);
extern "C" void casein_call_k(casein::event_type et, casein::keys k);
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons m);
extern "C" float * casein_screen_scale_factor;
