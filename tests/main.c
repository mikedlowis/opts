#include "atf.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    RUN_EXTERN_TEST_SUITE(Opts);
    return PRINT_TEST_RESULTS();
}
