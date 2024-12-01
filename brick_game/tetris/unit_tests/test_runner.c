#include "test_runner.h"

int main(void) {
  printf("\n");
  int nf = init_suite();
  nf = compare_suite();
  nf = fsm_suite();

  printf("\n___________ FAILED TESTS: %d ___________\n\n", nf);

  return 0;
}
