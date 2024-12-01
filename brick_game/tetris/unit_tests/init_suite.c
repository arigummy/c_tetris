#include <check.h>

#include "test_runner.h"

START_TEST(INIT_TEST_1) {
  init_game();
  GameInfo_t data = updateCurrentState();
  ck_assert_int_eq(data.level, 1);
  end_game();
}

int init_suite(void) {
  Suite *s = suite_create("Inition tests");
  TCase *tcCore = tcase_create("Core");
  SRunner *sr = srunner_create(s);
  int nf;

  tcase_add_test(tcCore, INIT_TEST_1);
  suite_add_tcase(s, tcCore);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
