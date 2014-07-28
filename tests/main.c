#include "test.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    RUN_TEST_SUITE(Opts);
    return PRINT_TEST_RESULTS();
}
