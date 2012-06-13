// Unit Test Framework Includes
#include "UnitTest++.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>

// File To Test
#include "opts.h"

using namespace UnitTest;

//-----------------------------------------------------------------------------
// Sample Option Configuration
//-----------------------------------------------------------------------------
OptionConfig_T Options_Config[] = {
    { SHORT, (char*)"a",   (char*)"test_a", 0, (char*)"A simple test option" },
    { SHORT, (char*)"b",   (char*)"test_b", 1, (char*)"A simple test option" },
    { LONG,  (char*)"foo", (char*)"test_d", 0, (char*)"A simple test option" },
    { LONG,  (char*)"bar", (char*)"test_e", 1, (char*)"A simple test option" },
    { END,   (char*)NULL,  (char*)NULL,     0, (char*)NULL }
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

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {
    //-------------------------------------------------------------------------
    // Test InitContext Function
    //-------------------------------------------------------------------------
    TEST(Verify_InitContext_Initializes_The_StreamContext)
    {
        StreamContext_T ctx;
        char* args[] = { (char*)"-a", (char*)"b", (char*)"--foo", (char*)"--foo=bar" };
        OPTS_InitContext( &ctx, 4, args );

        CHECK( 0 == ctx.line_idx );
        CHECK( 0 == ctx.col_idx );
        CHECK( 4 == ctx.arg_count );
        CHECK( args == ctx.arg_vect );
        CHECK( '-' == ctx.current );
        CHECK( NULL != ctx.results );
        CHECK( NULL != ctx.results->options );
        CHECK( NULL == ctx.results->options->head );
        CHECK( NULL == ctx.results->options->tail );
        CHECK( NULL != ctx.results->arguments );
        CHECK( NULL == ctx.results->arguments->head );
        CHECK( NULL == ctx.results->arguments->tail );
    }

    //-------------------------------------------------------------------------
    // Test ParseOptions Function
    //-------------------------------------------------------------------------
    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_No_Param)
    {
        char* args[] = { (char*)"-a" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 1, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "a" ) );
            CHECK( NULL == result->tail->val );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_A_Space)
    {
        char* args[] = { (char*)"-b", (char*)"5" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 2, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "b" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "5" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_No_Space)
    {
        char* args[] = { (char*)"-b5" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 1, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "b" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "5" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_No_Param)
    {
        char* args[] = { (char*)"--foo" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 1, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "foo" ) );
            CHECK( NULL == result->tail->val );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_A_Space)
    {
        char* args[] = { (char*)"--bar", (char*)"baz" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 2, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "bar" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "baz" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_An_Equals_Sign)
    {
        char* args[] = { (char*)"--bar=baz" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 1, args );

            OptionList_T* result = results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "bar" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "baz" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseOptions_Parses_A_Floating_Argument)
    {
        char* args[] = { (char*)"baz1" };

        CHECK_DOES_NOT_EXIT()
        {
            Result_T* results = OPTS_ParseOptions( Options_Config, 1, args );

            ArgumentList_T* result = results->arguments;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( 0 == strcmp( result->tail->val, (char*)"baz1" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    //-------------------------------------------------------------------------
    // Test ParseShortOption Function
    //-------------------------------------------------------------------------
    TEST(Verify_ParseShortOption_Exits_With_Error_On_Invalid_Option)
    {
        int exit_code = 0;
        char* args[] = { (char*)"z" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code )
        {
            OPTS_ParseShortOption( &ctx );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        }
        else
        {
            OptionList_T* result = ctx.results->options;
            CHECK( 1 == exit_code );
            CHECK( NULL != result );
            CHECK( NULL == result->head );
            CHECK( NULL == result->tail );
        }
    }

    TEST(Verify_ParseShortOption_Parses_A_Single_Valid_Option_With_No_Arg)
    {
        char* args[] = { (char*)"a", (char*)"a" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 2, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseShortOption( &ctx );
            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "a" ) );
            CHECK( NULL == result->tail->val );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseShortOption_Parses_A_Single_Valid_Option_With_Arg_And_No_Space)
    {
        char* args[] = { (char*)"b5" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseShortOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "b" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "5" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseShortOption_Parses_A_Single_Valid_Option_With_Arg_And_Space)
    {
        char* args[] = { (char*)"b", (char*)"5" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 2, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseShortOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "b" ) );
            CHECK( NULL != result->tail->val );
            CHECK( 0 == strcmp( result->tail->val, "5" ) );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseShortOption_Exits_With_Error_When_No_Arg_Received_When_Expected)
    {
        int exit_code = 0;
        char* args[] = { (char*)"b" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code )
        {
            OPTS_ParseShortOption( &ctx );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        }
        else
        {
            OptionList_T* result = ctx.results->options;
            CHECK( 1 == exit_code );
            CHECK( NULL != result );
            CHECK( NULL == result->head );
            CHECK( NULL == result->tail );
        }
    }

    TEST(Verify_ParseShortOption_Parses_A_Group_Of_Valid_Options_With_No_Arg)
    {
        char* args[] = { (char*)"aa" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseShortOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head != result->tail );
            // First option
            CHECK( NULL != result->head->key );
            CHECK( 0 == strcmp( result->head->key, "a" ) );
            CHECK( NULL == result->head->val );
            CHECK( NULL != result->head->next );
            // Second option
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "a" ) );
            CHECK( NULL == result->tail->val );
            CHECK( NULL == result->tail->next );
        }
    }

    TEST(Verify_ParseShortOption_Parses_A_Group_Of_Valid_Options_With_The_Last_Having_An_Arg)
    {
        char* args[] = { (char*)"ab5" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseShortOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head != result->tail );
            // First option
            CHECK( NULL != result->head->key );
            CHECK( 0 == strcmp( result->head->key, "b" ) );
            CHECK( NULL != result->head->val );
            CHECK( 0 == strcmp( result->head->val, "5" ) );
            CHECK( NULL != result->head->next );
            // Second option
            CHECK( NULL != result->tail->key );
            CHECK( 0 == strcmp( result->tail->key, "a" ) );
            CHECK( NULL == result->tail->val );
            CHECK( NULL == result->tail->next );
        }
    }

    //-------------------------------------------------------------------------
    // Test ParseLongOption Function
    //-------------------------------------------------------------------------
    TEST(Verify_ParseLongOption_Errors_And_Exits_On_Invalid_Option)
    {
        int exit_code = 0;
        char* args[] = { (char*)"baz" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code )
        {
            OPTS_ParseLongOption( &ctx );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        }
        else
        {
            OptionList_T* result = ctx.results->options;
            CHECK( 1 == exit_code );
            CHECK( NULL != result );
            CHECK( NULL == result->head );
            CHECK( NULL == result->tail );
        }
    }

    TEST(Verify_ParseLongOption_Errors_And_Exits_Next_Option_Instead_Of_Argument)
    {
        int exit_code = 0;
        char* args[] = { (char*)"bar", (char*)"--foo" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 2, args );

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code )
        {
            OPTS_ParseLongOption( &ctx );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        }
        else
        {
            OptionList_T* result = ctx.results->options;
            CHECK( 1 == exit_code );
            CHECK( NULL != result );
            CHECK( NULL == result->head );
            CHECK( NULL == result->tail );
        }
    }

    TEST(Verify_ParseLongOption_Errors_And_Exits_EOF_Instead_Of_Argument)
    {
        int exit_code = 0;
        char* args[] = { (char*)"bar" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        exit_code = setjmp( Exit_Point );
        if( 0 == exit_code )
        {
            OPTS_ParseLongOption( &ctx );
            // If we fail to call exit then this breaks our test
            CHECK( false );
        }
        else
        {
            OptionList_T* result = ctx.results->options;
            CHECK( 1 == exit_code );
            CHECK( NULL != result );
            CHECK( NULL == result->head );
            CHECK( NULL == result->tail );
        }
    }

    TEST(Verify_ParseLongOption_Parses_Option_With_No_Arg)
    {
        char* args[] = { (char*)"foo" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseLongOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( 0 == strcmp( result->head->key, (char*)"foo") );
            CHECK( NULL == result->tail->val );
        }
    }

    TEST(Verify_ParseLongOption_Parses_Option_With_Arg_Using_Equals_Sign)
    {
        char* args[] = { (char*)"bar=baz" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseLongOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( 0 == strcmp( result->head->key, (char*)"bar") );
            CHECK( 0 == strcmp( result->head->val, (char*)"baz") );
        }
    }

    TEST(Verify_ParseLongOption_Parses_Option_With_Arg_Not_Using_Equals_Sign)
    {
        char* args[] = { (char*)"bar", (char*)"baz" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 2, args );

        CHECK_DOES_NOT_EXIT()
        {
            OPTS_ParseLongOption( &ctx );

            OptionList_T* result = ctx.results->options;
            CHECK( NULL != result );
            CHECK( NULL != result->head );
            CHECK( NULL != result->tail );
            CHECK( result->head == result->tail );
            CHECK( 0 == strcmp( result->head->key, (char*)"bar") );
            CHECK( 0 == strcmp( result->head->val, (char*)"baz") );
        }
    }

    //-------------------------------------------------------------------------
    // Test ParseArgument Function
    //-------------------------------------------------------------------------
    TEST(Verify_ParseArgument_Does_Nothing_If_No_Tokens_Left)
    {
        char* args[] = { (char*)"" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        OPTS_ParseArgument( &ctx );

        ArgumentList_T* result = ctx.results->arguments;
        CHECK( NULL != result );
        CHECK( NULL == result->head );
        CHECK( NULL == result->tail );
    }

    TEST(Verify_ParseArgument_Adds_Argument_To_Empty_List)
    {
        char* args[] = { (char*)"baz1" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );

        OPTS_ParseArgument( &ctx );

        ArgumentList_T* result = ctx.results->arguments;
        CHECK( NULL != result );
        CHECK( NULL != result->head );
        CHECK( NULL != result->tail );
        CHECK( result->head == result->tail );
        CHECK( 0 == strcmp( result->tail->val, (char*)"baz1" ) );
        CHECK( NULL == result->tail->next );
    }

    TEST(Verify_ParseArgument_Appends_Argument_To_End_Of_List)
    {
        char* args[] = { (char*)"baz1" };
        StreamContext_T ctx;
        ctx.options = Options_Config;
        OPTS_InitContext( &ctx, 1, args );
        ArgumentList_T* result = ctx.results->arguments;
        Argument_T arg = { 0 };
        result->head = &arg;
        result->tail = &arg;

        OPTS_ParseArgument( &ctx );

        CHECK( NULL != result );
        CHECK( NULL != result->head );
        CHECK( NULL != result->tail );
        CHECK( result->head != result->tail );
        CHECK( 0 == strcmp( result->tail->val, (char*)"baz1" ) );
        CHECK( NULL == result->tail->next );
    }

    //-------------------------------------------------------------------------
    // Test GetOptConfig Function
    //-------------------------------------------------------------------------
    TEST(Verify_GetOptConfig_Can_Retrieve_A_Short_Config_By_Name_And_Type)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, SHORT, (char*)"a" );
        CHECK( result == &(Options_Config[0]) );
    }

    TEST(Verify_GetOptConfig_Can_Retrieve_A_Long_Config_By_Name_And_Type)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, LONG, (char*)"foo" );
        CHECK( result == &(Options_Config[2]) );
    }

    TEST(Verify_GetOptConfig_Returns_Null_If_No_Config_Found)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, LONG, (char*)"baz" );
        CHECK( result == NULL );
    }

    TEST(Verify_GetOptConfig_Returns_Null_If_Config_List_Empty)
    {
        OptionConfig_T config[] = {
            { END,   NULL,  NULL,     0, NULL }
        };
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( config, LONG, (char*)"foo" );
        CHECK( result == NULL );
    }

    //-------------------------------------------------------------------------
    // Test AddOption Function
    //-------------------------------------------------------------------------
    TEST(Verify_AddOption_Ignores_NULL_Result_Set)
    {
        Result_T results;
        results.options = NULL;

        // If we don't segfault here I call it a win
        OPTS_AddOption( NULL, NULL, NULL );
        OPTS_AddOption( &results, NULL, NULL );
    }

    TEST(Verify_AddOption_Adds_Result_To_Empty_List)
    {
        Result_T results;
        char name[] = "foo";
        char arg[]  = "bar";
        results.options = (OptionList_T*)malloc( sizeof( OptionList_T ) );
        results.options->head = NULL;
        results.options->tail = NULL;
        OPTS_AddOption( &results, name, arg );

        CHECK( NULL != results.options->head );
        CHECK( NULL != results.options->tail );
        CHECK( NULL != results.options->head->key );
        CHECK( name != results.options->head->key );
        CHECK( 0 == strcmp( name, results.options->head->key ) );
        CHECK( NULL != results.options->head->val );
        CHECK( arg == results.options->head->val );
        CHECK( NULL == results.options->head->next );
        CHECK( results.options->head == results.options->tail );

        delete results.options->head->key;
        delete results.options->head;
        delete results.options;
    }

    TEST(Verify_AddOption_Appends_Result_To_Tail_Of_List)
    {
        Result_T results;
        Option_T option = {0};
        char name[] = "foo";
        char arg[]  = "bar";
        results.options = (OptionList_T*)malloc( sizeof( OptionList_T ) );
        results.options->head = &option;
        results.options->tail = &option;
        OPTS_AddOption( &results, name, arg );

        CHECK( NULL != results.options->head );
        CHECK( NULL != results.options->tail );
        CHECK( results.options->head != results.options->tail );
        CHECK( NULL != results.options->tail->key );
        CHECK( name != results.options->tail->key );
        CHECK( 0 == strcmp( name, results.options->tail->key ) );
        CHECK( NULL != results.options->tail->val );
        CHECK( arg == results.options->tail->val );
        CHECK( NULL == results.options->tail->next );

        delete results.options->tail->key;
        delete results.options->tail;
        delete results.options;
    }

    //-------------------------------------------------------------------------
    // Test ConsumeWhitespaace Function
    //-------------------------------------------------------------------------
    TEST(Verify_ConsumeWhitespace_Removes_Whitespace_From_The_Stream_Till_The_Next_Token)
    {
        char* args[] = { (char*)"    " };
        StreamContext_T ctx;
        ctx.line_idx  = 0;
        ctx.col_idx   = -1;
        ctx.arg_count = 1;
        ctx.arg_vect  = args;
        (void)OPTS_NextCharacter( &ctx );

        OPTS_ConsumeWhitespace( &ctx );
        CHECK( EOF == OPTS_NextCharacter( &ctx ) );
    }

    //-------------------------------------------------------------------------
    // Test NextToken Function
    //-------------------------------------------------------------------------
    TEST(Verify_NextToken_Properly_Tokenizes_Stream)
    {
        char* token = NULL;
        char* args[] = { (char*)"-a", (char*)"b", (char*)"--foo", (char*)"--foo=bar" };
        StreamContext_T ctx;
        ctx.line_idx  = 0;
        ctx.col_idx   = -1;
        ctx.arg_count = 4;
        ctx.arg_vect  = args;

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, (char*)"-a" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, (char*)"b" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, (char*)"--foo" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, (char*)"--foo" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, (char*)"bar" ) );
        free( token );
    }

    //-------------------------------------------------------------------------
    // Test NextCharacter Function
    //-------------------------------------------------------------------------
    TEST(Verify_NextCharacter_Can_Loop_Through_Characters)
    {
        char* args[] = { (char*)"abc", (char*)"123=4" };
        StreamContext_T ctx;
        ctx.line_idx  = 0;
        ctx.col_idx   = -1;
        ctx.arg_count = 2;
        ctx.arg_vect  = args;

        CHECK( 'a' == OPTS_NextCharacter( &ctx ) );
        CHECK( 'a' == ctx.current );
        CHECK( 'b' == OPTS_NextCharacter( &ctx ) );
        CHECK( 'b' == ctx.current );
        CHECK( 'c' == OPTS_NextCharacter( &ctx ) );
        CHECK( 'c' == ctx.current );
        CHECK( ' ' == OPTS_NextCharacter( &ctx ) );
        CHECK( ' ' == ctx.current );
        CHECK( '1' == OPTS_NextCharacter( &ctx ) );
        CHECK( '1' == ctx.current );
        CHECK( '2' == OPTS_NextCharacter( &ctx ) );
        CHECK( '2' == ctx.current );
        CHECK( '3' == OPTS_NextCharacter( &ctx ) );
        CHECK( '3' == ctx.current );
        CHECK( ' ' == OPTS_NextCharacter( &ctx ) );
        CHECK( ' ' == ctx.current );
        CHECK( '4' == OPTS_NextCharacter( &ctx ) );
        CHECK( '4' == ctx.current );
        CHECK( ' ' == OPTS_NextCharacter( &ctx ) );
        CHECK( ' ' == ctx.current );
        CHECK( EOF == OPTS_NextCharacter( &ctx ) );
        CHECK( EOF == ctx.current );
        CHECK( EOF == OPTS_NextCharacter( &ctx ) );
        CHECK( EOF == ctx.current );
    }

    TEST(Verify_NextCharacter_Can_Handle_Empty_Input)
    {
        char* args[] = {};
        StreamContext_T ctx;
        ctx.line_idx  = 0;
        ctx.col_idx   = -1;
        ctx.arg_count = 0;
        ctx.arg_vect  = args;

        CHECK( EOF == OPTS_NextCharacter( &ctx ) );
        CHECK( EOF == ctx.current );
        CHECK( EOF == OPTS_NextCharacter( &ctx ) );
        CHECK( EOF == ctx.current );
    }

    //-------------------------------------------------------------------------
    // Test AppendCharacter Function
    //-------------------------------------------------------------------------
    TEST(Verify_AppendCharacter_Dynamically_Resizes_and_Appends_Char)
    {
        char* str = (char*)malloc(2);
        str[0] = 'a';
        str[1] = '\0';
        str = OPTS_AppendCharacter( str, 'b' );

        CHECK( 0 == strcmp( str, (char*)"ab" ) );
    }
}
