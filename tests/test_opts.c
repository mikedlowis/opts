// Unit Test Framework Includes
#include "atf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdbool.h>

// File To Test
#include "opts.h"

//-----------------------------------------------------------------------------
// Sample Option Configuration
//-----------------------------------------------------------------------------
opts_cfg_t Options_Config[] = {
    { "a",   false, "test_a", "A simple test option" },
    { "b",   true,  "test_b", "A simple test option" },
    { "c",   false, "test_c", "A simple test option" },
    { "foo", false, "opttag", "A simple test option" },
    { "bar", true,  "test_e", "A simple test option" },
    { "baz", false, "opttag", "A simple test option" },
    { NULL,  false, NULL,     NULL }
};

//-----------------------------------------------------------------------------
// Global Test Variables
//-----------------------------------------------------------------------------
static jmp_buf Exit_Point = {0};

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
#define CHECK_DOES_NOT_EXIT() \
    if( 0 != setjmp(Exit_Point) ) { CHECK( false ); } else

void exit(int code)
{
    // Simulate an exit by longjmping to the jmp_buf with the desired error code
    longjmp( Exit_Point, code);
}


static void User_Error_Cb(const char* msg, char* opt_name) {
    (void)msg;
    (void)opt_name;
    exit(2);
}

void test_setup(void) {}

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
TEST_SUITE(Opts) {
    //-------------------------------------------------------------------------
    // Test ParseOptions Function
    //-------------------------------------------------------------------------
    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_No_Param)
    {
        char* args[] = { "prog", (char*)"--foo" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("foo", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_A_Space)
    {
        char* args[] = { "prog", (char*)"--bar", (char*)"baz" };

        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 3, args );
            CHECK(opts_is_set("bar", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("bar", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_An_Equals_Sign)
    {
        char* args[] = { "prog", (char*)"--bar=baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("bar", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("bar", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_long_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"--bar" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_long_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"--bar-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_exits_when_invalid_long_option)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"--baz-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_short_option_with_no_argument)
    {
        char* args[] = { "prog", (char*)"-a" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("a", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_short_option_list_with_no_args)
    {
        char* args[] = { "prog", (char*)"-ac" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("a", NULL));
            CHECK(opts_is_set("c", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument)
    {
        char* args[] = { "prog", (char*)"-bbaz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument_and_a_space)
    {
        char* args[] = { "prog", (char*)"-b", "baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 3, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument_and_an_equals)
    {
        char* args[] = { "prog", (char*)"-b=baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }


    TEST(Verify_ParseOptions_Parses_a_short_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"-b" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"-b-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_exits_when_invalid_short_option)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"-d" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, NULL, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Floating_Argument)
    {
        char* args[] = { "prog", (char*)"baz1" };

        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 2, args );
            const char** args = opts_arguments();
            CHECK(0 == strcmp("baz1", args[0]));
            CHECK(NULL == args[1]);
            free(args);
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_Multiple_Floating_Arguments)
    {
        char* args[] = { "prog", "baz1", "baz2" };

        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 3, args );
            const char** args = opts_arguments();
            CHECK(0 == strcmp("baz2", args[0]));
            CHECK(0 == strcmp("baz1", args[1]));
            CHECK(NULL == args[2]);
            free(args);
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Calls_the_user_defined_error_handler_if_provided)
    {
        int exit_code = 0;
        char* args[] = { "prog", (char*)"-b" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, User_Error_Cb, 2, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 2 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_Select_returns_the_matching_option_by_name)
    {
        char* args[] = { "prog", "-a", "--foo", "-c" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 4, args );
            const char** opts = opts_select("c", NULL);
            CHECK(0 == strcmp("c", opts[0]));
            CHECK(NULL == opts[1]);
            free(opts);
        }
        opts_reset();
    }

    TEST(Verify_Select_returns_the_matching_option_by_tag)
    {
        char* args[] = { "prog", "-a", "--foo", "-c" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 4, args );
            const char** opts = opts_select(NULL, "test_c");
            CHECK(0 == strcmp("c", opts[0]));
            CHECK(NULL == opts[1]);
            free(opts);
        }
        opts_reset();
    }

    TEST(Verify_Select_returns_the_matching_option_by_name_and_tag)
    {
        char* args[] = { "prog", "-a", "--foo", "-c", "--baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 5, args );
            const char** opts = opts_select("baz", "opttag");
            CHECK(0 == strcmp("baz", opts[0]));
            CHECK(NULL == opts[1]);
            free(opts);
        }
        opts_reset();
    }

    TEST(Verify_Select_returns_the_matching_options_by_tag)
    {
        char* args[] = { "prog", "-a", "--foo", "-c", "--baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, NULL, 5, args );
            const char** opts = opts_select(NULL, "opttag");
            CHECK(0 == strcmp("baz", opts[0]));
            CHECK(0 == strcmp("foo", opts[1]));
            CHECK(NULL == opts[2]);
            free(opts);
        }
        opts_reset();
    }
}
