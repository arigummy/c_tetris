#include <check.h>

#include "test_runner.h"

START_TEST(VERTICAL_COLLISION_TEST_1) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = 5;
  ck_assert_int_eq(vertical_collision(data), 0);
}

START_TEST(VERTICAL_COLLISION_TEST_2) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.x = 0;
  ck_assert_int_eq(vertical_collision(data), -1);
}

START_TEST(VERTICAL_COLLISION_TEST_3) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.x = 7;
  ck_assert_int_eq(vertical_collision(data), 1);
}

START_TEST(HORIZLONTAL_COLLISION_TEST_1) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = 5;
  ck_assert_int_eq(horizontal_collision(data), 0);
}

START_TEST(HORIZLONTAL_COLLISION_TEST_2) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = 19;
  ck_assert_int_eq(horizontal_collision(data), 1);
}

START_TEST(ALL_COLLISION_TEST_1) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = 5;
  ck_assert_int_eq(all_collision(data), 0);
}

START_TEST(ALL_COLLISION_TEST_2) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = -2;
  ck_assert_int_eq(all_collision(data), 1);
}

START_TEST(ALL_COLLISION_TEST_3) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.y = 19;
  ck_assert_int_eq(all_collision(data), 1);
}

START_TEST(ALL_COLLISION_TEST_4) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.x = -2;
  ck_assert_int_eq(all_collision(data), 1);
}

START_TEST(ALL_COLLISION_TEST_5) {
  init_game();
  game_data_t data = get_all_data();
  data.figure.x = 10;
  ck_assert_int_eq(all_collision(data), 1);
}

int compare_suite(void) {
  Suite *s = suite_create("Compares tests");
  TCase *tcCore = tcase_create("Core");
  SRunner *sr = srunner_create(s);
  int nf;

  tcase_add_test(tcCore, VERTICAL_COLLISION_TEST_1);
  tcase_add_test(tcCore, VERTICAL_COLLISION_TEST_2);
  tcase_add_test(tcCore, VERTICAL_COLLISION_TEST_3);
  tcase_add_test(tcCore, HORIZLONTAL_COLLISION_TEST_1);
  tcase_add_test(tcCore, HORIZLONTAL_COLLISION_TEST_2);
  tcase_add_test(tcCore, ALL_COLLISION_TEST_1);
  tcase_add_test(tcCore, ALL_COLLISION_TEST_2);
  tcase_add_test(tcCore, ALL_COLLISION_TEST_3);
  tcase_add_test(tcCore, ALL_COLLISION_TEST_4);
  tcase_add_test(tcCore, ALL_COLLISION_TEST_5);
  suite_add_tcase(s, tcCore);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
