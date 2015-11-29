/**
  @file test.h
  @brief TODO: Describe this file
  $Revision$
  $HeadURL$
  */
#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define CHECK(expr) \
    if (!(expr)) { atf_test_fail(#expr,__FILE__,__LINE__); break; }

#define TEST_SUITE(name) void name(void)

#define TEST(desc) \
    for(atf_test_start(#desc); Curr_Test != NULL; Curr_Test = NULL)

#define RUN_EXTERN_TEST_SUITE(name) \
    do { extern TEST_SUITE(name); atf_run_suite(&name); } while(0)

#define RUN_TEST_SUITE(name) \
    do { atf_run_suite(&name) } while(0)

#define PRINT_TEST_RESULTS atf_print_results

typedef void (*suite_t)(void);

extern char* Curr_Test;

void atf_run_suite(suite_t suite);

void atf_test_start(char* p_test_name);

void atf_test_fail(char* expr, char* file, int line);

int atf_print_results(void);

#endif /* TEST_H */
