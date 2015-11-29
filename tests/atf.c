/**
  @file atf.c
  @brief See header for details
  $Revision$
  $HeadURL$
  */
#include "atf.h"

char* Curr_Test = NULL;
static unsigned int Total = 0;
static unsigned int Failed = 0;

void atf_run_suite(suite_t suite) {
    suite();
}

void atf_test_start(char* p_test_name) {
    Curr_Test = p_test_name;
    Total++;
}

void atf_test_fail(char* expr, char* file, int line) {
    Failed++;
    printf("%s:%d:0:%s:FAIL\n\t%s\n", file, line, Curr_Test, expr); \
}

int atf_print_results(void) {
    static const char* results_string =
    "\nUnit Test Summary"
    "\n-----------------"
    "\nTotal:  %d"
    "\nPassed: %d"
    "\nFailed: %d"
    "\n\n";
    printf(results_string, Total, Total - Failed, Failed);
    return Failed;
}

