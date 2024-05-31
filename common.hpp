#pragma once

namespace casein {
  inline constexpr const auto base_width = 1280;
  inline constexpr const auto base_height = 720;

  enum event_type {
    CREATE_WINDOW,
    GESTURE,
    KEY_DOWN,
    KEY_UP,
    MOUSE_DOWN,
    MOUSE_MOVE,
    MOUSE_MOVE_REL,
    MOUSE_UP,
    REPAINT,
    RESIZE_WINDOW,
    QUIT,
    TIMER,
    TOUCH_CANCEL,
    TOUCH_DOWN,
    TOUCH_MOVE,
    TOUCH_UP,
    MAX_EVENT_TYPE,
  };
  enum keys : unsigned {
    K_NULL,
    K_SPACE = ' ',
    K_DOT = '.',
    K_0 = '0',
    K_1 = '1',
    K_2 = '2',
    K_3 = '3',
    K_4 = '4',
    K_5 = '5',
    K_6 = '6',
    K_7 = '7',
    K_8 = '8',
    K_9 = '9',
    K_A = 'a',
    K_B = 'b',
    K_C = 'c',
    K_D = 'd',
    K_E = 'e',
    K_F = 'f',
    K_G = 'g',
    K_H = 'h',
    K_I = 'i',
    K_J = 'j',
    K_K = 'k',
    K_L = 'l',
    K_M = 'm',
    K_N = 'n',
    K_O = 'o',
    K_P = 'p',
    K_Q = 'q',
    K_R = 'r',
    K_S = 's',
    K_T = 't',
    K_U = 'u',
    K_V = 'v',
    K_W = 'w',
    K_X = 'x',
    K_Y = 'y',
    K_Z = 'z',
    // Aliases
    K_PERIOD = K_DOT,
    // Non-ASCII
    K_DOWN = 0x100,
    K_ENTER,
    K_ESCAPE,
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_F1 = 0x110,
    K_F2,
    K_F3,
    K_F4,
    K_F5,
    K_F6,
    K_F7,
    K_F8,
    K_F9,
    K_F10,
    K_F11,
    K_F12,
    K_MAX,
  };
  enum mouse_buttons {
    M_LEFT,
    M_RIGHT,
    M_MAX,
  };
  enum gestures {
    G_TAP_1,
    G_TAP_2,
    G_SHAKE,
    G_SWIPE_DOWN,
    G_SWIPE_LEFT,
    G_SWIPE_RIGHT,
    G_SWIPE_UP,
    G_MAX,
  };
  struct point {
    float x;
    float y;
  };
}

extern "C" void casein_call(casein::event_type et);
extern "C" void casein_call_g(casein::event_type et, casein::gestures g);
extern "C" void casein_call_k(casein::event_type et, casein::keys k);
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons m);
extern "C" float * casein_screen_scale_factor;
