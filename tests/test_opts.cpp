// Unit Test Framework Includes
#include "UnitTest++.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// File To Test
#include "opts.h"

using namespace UnitTest;

//-----------------------------------------------------------------------------
// Sample Option Configuration
//-----------------------------------------------------------------------------
OptionConfig_T Options_Config[] = {
    { SHORT, "a",   "test_a", 0, "A simple test option" },
    { SHORT, "b",   "test_b", 1, "A simple test option" },
    { LONG,  "foo", "test_d", 0, "A simple test option" },
    { LONG,  "bar", "test_e", 1, "A simple test option" },
    { END,   NULL,  NULL,     0, NULL }
};

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
        char* args[] = { "-a", "b", "--foo", "--foo=bar" };
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
    //TEST(Verify_ParseOptions_Parses_A_Short_Option_With_No_Param)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "-a" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}

    //TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_A_Space)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "-b5" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}

    //TEST(Verify_ParseOptions_Parses_A_Short_Option_With_A_Param_And_No_Space)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "-b", "5" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}

    //TEST(Verify_ParseOptions_Parses_A_Long_Option_With_No_Param)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "--foo" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}

    //TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_A_Space)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "--bar=5" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}

    //TEST(Verify_ParseOptions_Parses_A_Long_Option_With_A_Param_And_An_Equals_Sign)
    //{
    //    Result_T* results = NULL;
    //    char* args[] = { "--bar", "5" };
    //    results = OPTS_ParseOptions( Options_Config, 1, args );
    //    CHECK( 0 == 1 );
    //    free(results);
    //}


    //-------------------------------------------------------------------------
    // Test GetOptConfig Function
    //-------------------------------------------------------------------------
    TEST(Verify_GetOptConfig_Can_Retrieve_A_Short_Config_By_Name_And_Type)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, SHORT, "a" );
        CHECK( result == &(Options_Config[0]) );
    }

    TEST(Verify_GetOptConfig_Can_Retrieve_A_Long_Config_By_Name_And_Type)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, LONG, "foo" );
        CHECK( result == &(Options_Config[2]) );
    }

    TEST(Verify_GetOptConfig_Returns_Null_If_No_Config_Found)
    {
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( Options_Config, LONG, "baz" );
        CHECK( result == NULL );
    }

    TEST(Verify_GetOptConfig_Returns_Null_If_Config_List_Empty)
    {
        OptionConfig_T config[] = {
            { END,   NULL,  NULL,     0, NULL }
        };
        OptionConfig_T* result = NULL;
        result = OPTS_GetOptConfig( config, LONG, "foo" );
        CHECK( result == NULL );
    }

    //-------------------------------------------------------------------------
    // Test NextToken Function
    //-------------------------------------------------------------------------
    TEST(Verify_NextToken_Properly_Tokenizes_Stream)
    {
        char* token = NULL;
        char* args[] = { "-a", "b", "--foo", "--foo=bar" };
        StreamContext_T ctx;
        ctx.line_idx  = 0;
        ctx.col_idx   = -1;
        ctx.arg_count = 4;
        ctx.arg_vect  = args;

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, "-a" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, "b" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, "--foo" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, "--foo" ) );
        free( token );

        (void)OPTS_NextCharacter( &ctx );
        token = OPTS_NextToken( &ctx );
        CHECK( 0 == strcmp( token, "bar" ) );
        free( token );
    }

    //-------------------------------------------------------------------------
    // Test NextCharacter Function
    //-------------------------------------------------------------------------
    TEST(Verify_NextCharacter_Can_Loop_Through_Characters)
    {
        char* args[] = { "abc", "123" };
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

        CHECK( 0 == strcmp( str, "ab" ) );
    }
}
