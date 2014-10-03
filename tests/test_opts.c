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
OptionConfig_T Options_Config[] = {
    { (char*)"a",   false, (char*)"test_a", (char*)"A simple test option" },
    { (char*)"b",   true,  (char*)"test_b", (char*)"A simple test option" },
    { (char*)"c",   false, (char*)"test_c", (char*)"A simple test option" },
    { (char*)"foo", false, (char*)"test_d", (char*)"A simple test option" },
    { (char*)"bar", true,  (char*)"test_e", (char*)"A simple test option" },
    { (char*)NULL,  false, (char*)NULL,     (char*)NULL }
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
        char* args[] = { (char*)"--foo" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("foo", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_A_Space)
    {
        char* args[] = { (char*)"--bar", (char*)"baz" };

        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 2, args );
            CHECK(opts_is_set("bar", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("bar", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_An_Equals_Sign)
    {
        char* args[] = { (char*)"--bar=baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("bar", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("bar", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_long_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { (char*)"--bar" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
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
        char* args[] = { (char*)"--bar-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
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
        char* args[] = { (char*)"--baz-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_short_option_with_no_argument)
    {
        char* args[] = { (char*)"-a" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("a", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_short_option_list_with_no_args)
    {
        char* args[] = { (char*)"-ac" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("a", NULL));
            CHECK(opts_is_set("c", NULL));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument)
    {
        char* args[] = { (char*)"-bbaz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument_and_a_space)
    {
        char* args[] = { (char*)"-b", "baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 2, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_a_short_option_with_an_argument_and_an_equals)
    {
        char* args[] = { (char*)"-b=baz" };
        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
            CHECK(opts_is_set("b", NULL));
            CHECK(0 == strcmp("baz", opts_get_value("b", NULL)));
        }
        opts_reset();
    }


    TEST(Verify_ParseOptions_Parses_a_short_option_with_a_missing_param)
    {
        int exit_code = 0;
        char* args[] = { (char*)"-b" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
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
        char* args[] = { (char*)"-b-" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
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
        char* args[] = { (char*)"-d" };

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code ) {
            opts_parse( Options_Config, 1, args );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        } else {
            CHECK( 1 == exit_code );
        }
        opts_reset();
    }

    TEST(Verify_ParseOptions_Parses_A_Floating_Argument)
    {
        char* args[] = { (char*)"baz1" };

        CHECK_DOES_NOT_EXIT()
        {
            opts_parse( Options_Config, 1, args );
        }
        opts_reset();
    }












//    TEST(Verify_ParseOptions_exits_when_a_short_option_lacks_the_required_option)
//    {
//        Result_T* results;
//        int exit_code = 0;
//        char* args[] = { (char*)"-b-" };
//
//        exit_code = setjmp( Exit_Point );
//        if( 0 == exit_code ) {
//            results = opts_parse( Options_Config, 1, args );
//            // If we fail to call exit then this breaks our test
//            CHECK( false );
//        } else {
//            CHECK( 1 == exit_code );
//        }
//    }
//
//    TEST(Verify_ParseOptions_exits_when_a_short_option_with_no_accompanying_option)
//    {
//        Result_T* results;
//        int exit_code = 0;
//        char* args[] = { (char*)"-b" };
//
//        exit_code = setjmp( Exit_Point );
//        if( 0 == exit_code ) {
//            results = opts_parse( Options_Config, 1, args );
//            // If we fail to call exit then this breaks our test
//            CHECK( false );
//        } else {
//            CHECK( 1 == exit_code );
//        }
//    }
//
//    TEST(Verify_ParseOptions_errors_on_unknown_option)
//    {
//        Result_T* results;
//        int exit_code = 0;
//        char* args[] = { (char*)"-z" };
//
//        exit_code = setjmp( Exit_Point );
//        if( 0 == exit_code ) {
//            results = opts_parse( Options_Config, 1, args );
//            // If we fail to call exit then this breaks our test
//            CHECK( false );
//        } else {
//            CHECK( 1 == exit_code );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_short_option_group)
//    {
//        char* args[] = { (char*)"-aa" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 1, args );
//
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head != result->tail );
//            CHECK( NULL != result->head->key );
//            CHECK( 0 == strcmp( result->head->key, "a" ) );
//            CHECK( NULL == result->head->val );
//            CHECK( NULL != result->tail->key );
//            CHECK( 0 == strcmp( result->tail->key, "a" ) );
//            CHECK( NULL == result->tail->val );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_short_option_group_with_more_than_2_options)
//    {
//        char* args[] = { (char*)"-aaaa" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 1, args );
//
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head != result->tail );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_short_option_group_with_the_last_having_an_arg)
//    {
//        char* args[] = { (char*)"-ab5" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 1, args );
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head != result->tail );
//            CHECK( NULL != result->head->key );
//            CHECK( 0 == strcmp( result->head->key, "b" ) );
//            CHECK( NULL != result->head->val );
//            CHECK( 0 == strcmp( result->head->val, "5" ) );
//            CHECK( NULL != result->tail->key );
//            CHECK( 0 == strcmp( result->tail->key, "a" ) );
//            CHECK( NULL == result->tail->val );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_No_Param)
//    {
//        char* args[] = { (char*)"-a" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 1, args );
//
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head == result->tail );
//            CHECK( NULL != result->tail->key );
//            CHECK( 0 == strcmp( result->tail->key, "a" ) );
//            CHECK( NULL == result->tail->val );
//            CHECK( NULL == result->tail->next );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_A_Space)
//    {
//        char* args[] = { (char*)"-b", (char*)"5" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 2, args );
//
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head == result->tail );
//            CHECK( NULL != result->tail->key );
//            CHECK( 0 == strcmp( result->tail->key, "b" ) );
//            CHECK( NULL != result->tail->val );
//            CHECK( 0 == strcmp( result->tail->val, "5" ) );
//            CHECK( NULL == result->tail->next );
//        }
//    }
//
//    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_No_Space)
//    {
//        char* args[] = { (char*)"-b5" };
//
//        CHECK_DOES_NOT_EXIT()
//        {
//            Result_T* results = opts_parse( Options_Config, 1, args );
//
//            OptionList_T* result = results->options;
//            CHECK( NULL != result );
//            CHECK( NULL != result->head );
//            CHECK( NULL != result->tail );
//            CHECK( result->head == result->tail );
//            CHECK( NULL != result->tail->key );
//            CHECK( 0 == strcmp( result->tail->key, "b" ) );
//            CHECK( NULL != result->tail->val );
//            CHECK( 0 == strcmp( result->tail->val, "5" ) );
//            CHECK( NULL == result->tail->next );
//        }
//    }
}
