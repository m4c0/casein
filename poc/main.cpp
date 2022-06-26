#include "casein.hpp"

void casein_event(const casein::event & e) {
  switch (e.type()) {
  case casein::START:
  case casein::QUIT:
    break;
  default:
    break;
  }
}
