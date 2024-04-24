export module casein:call;

#include "common.hpp"

namespace casein {
  void call(event_type et);
  void call_g(event_type et, gestures g);
  void call_k(event_type et, keys k);
  void call_m(event_type et, mouse_buttons m);
}
