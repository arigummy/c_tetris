#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <check.h>
#include <stdio.h>
#include "../includes/s21_tetris_lib.h"

int init_suite(void);
int compare_suite(void);
int fsm_suite(void);
int others_funcs_suite(void);

#endif
