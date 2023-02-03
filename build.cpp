#include "build.hpp"

int main(int argc, char ** argv) {
  using namespace ecow;
  auto m = casein();

  seq all { "all" };
  all.add_ref(m);

  auto poc = all.add_unit<app>("poc");
  poc->add_unit<>("poc");
  poc->add_ref(m);

  return run_main(all, argc, argv);
}
