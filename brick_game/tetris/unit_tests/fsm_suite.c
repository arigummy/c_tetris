#include <check.h>

#include "test_runner.h"

START_TEST(FALLING_TEST_1) {
  init_game();
  set_sig(SPACE_BTN);
  set_state(MOVING);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, ATTACHING);
  end_game();
}

START_TEST(ROTATE_TEST_1) {
  init_game();
  set_sig(LEFT_BTN);
  set_state(MOVING);
  updateCurrentState();
  set_sig(LEFT_BTN);
  set_state(MOVING);
  updateCurrentState();
  set_sig(LEFT_BTN);
  set_state(MOVING);
  updateCurrentState();
  set_sig(LEFT_BTN);
  set_state(MOVING);
  updateCurrentState();
  set_sig(LEFT_BTN);
  set_state(MOVING);
  updateCurrentState();
  set_sig(UP_BTN);
  set_state(MOVING);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(all_collision(data), 0);
  end_game();
}

START_TEST(SHIFTING_TEST_1) {
  init_game();
  set_sig(NOSIG);
  set_state(SHIFTING);
  updateCurrentState();
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, MOVING);
  end_game();
}

START_TEST(ATTACHING_TEST_1) {
  init_game();
  set_sig(SPACE_BTN);
  set_state(MOVING);
  updateCurrentState();
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, SPAWN);
  end_game();
}

START_TEST(ATTACHING_TEST_2) {
  init_game();
  set_sig(SPACE_BTN);
  set_state(ATTACHING);
  set_pattern_1();
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.ground[19][2], 3);
  end_game();
}

START_TEST(GAMEOVER_TEST_1) {
  init_game();
  set_sig(SPACE_BTN);
  set_state(GAMEOVER);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.info.level, 0);
  end_game();
}

START_TEST(TIMEOUT_TEST_1) {
  init_game();
  set_sig(PAUSE_BTN);
  set_state(MOVING);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, PAUSE);
  end_game();
}

START_TEST(TIMEOUT_TEST_2) {
  init_game();
  set_sig(NOSIG);
  set_state(PAUSE);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, PAUSE);
  end_game();
}

START_TEST(TIMEOUT_TEST_3) {
  init_game();
  set_sig(SPACE_BTN);
  set_state(PAUSE);
  updateCurrentState();
  game_data_t data = get_all_data();
  ck_assert_int_eq(data.state, MOVING);
  end_game();
}

int fsm_suite(void) {
  Suite *s = suite_create("FSM tests");
  TCase *tcCore = tcase_create("Core");
  SRunner *sr = srunner_create(s);
  int nf;

  tcase_add_test(tcCore, FALLING_TEST_1);
  tcase_add_test(tcCore, ROTATE_TEST_1);
  tcase_add_test(tcCore, SHIFTING_TEST_1);
  tcase_add_test(tcCore, ATTACHING_TEST_1);
  tcase_add_test(tcCore, ATTACHING_TEST_2);
  tcase_add_test(tcCore, GAMEOVER_TEST_1);
  tcase_add_test(tcCore, TIMEOUT_TEST_1);
  tcase_add_test(tcCore, TIMEOUT_TEST_2);
  tcase_add_test(tcCore, TIMEOUT_TEST_3);
  suite_add_tcase(s, tcCore);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
