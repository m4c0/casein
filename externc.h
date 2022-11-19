#pragma once

extern "C" {
void casein_create_window(void * ptr);
void casein_key_down(int key);
void casein_key_up(int key);
void casein_mouse_down(int btn);
void casein_mouse_move(int x, int y);
void casein_mouse_up(int btn);
void casein_quit();
void casein_repaint();
}
