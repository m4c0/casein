#pragma once
#include "keys.hpp"

extern "C" {
void casein_create_window(void * ptr);
void casein_key_down(keys key);
void casein_key_up(keys key);
void casein_mouse_down(int x, int y, int btn);
void casein_mouse_move(int x, int y);
void casein_mouse_up(int x, int y, int btn);
void casein_quit();
void casein_repaint();
void casein_resize_window(int w, int h, float scale, bool live);
}
