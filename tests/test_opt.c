// Unit Test Framework Includes
#include "atf.h"
#include <stdbool.h>
#include <string.h>

// File To Test
#include "opt.h"

char* ARGV0;
int argc;
char** argv;

bool Aborted = false;
void abort(void) {
    Aborted = true;
}

void dummy(){}

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
TEST_SUITE(Arg) {
    //-------------------------------------------------------------------------
    // Short Option Parsing
    //-------------------------------------------------------------------------
    TEST(should parse an option with no argument)
    {
        char* args[] = { "prog", "-a", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a': CHECK(true); break;
            default:  CHECK(false);
        } OPTEND;
    }

    TEST(should parse two short options grouped together)
    {
        char* args[] = { "prog", "-ab", NULL };
        bool a = false, b = false;
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a': a = true; break;
            case 'b': b = true; break;
            default:  CHECK(false);
        } OPTEND;
        CHECK(a && b);
    }

    TEST(should parse three short options grouped together)
    {
        char* args[] = { "prog", "-abc", NULL };
        bool a = false, b = false, c = false;
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a': a = true; break;
            case 'b': b = true; break;
            case 'c': c = true; break;
            default:  CHECK(false);
        } OPTEND;
        CHECK(a && b && c);
    }

    TEST(OPTARG should parse an option with an adjacent argument)
    {
        char* args[] = { "prog", "-afoo", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                CHECK(0 == strcmp("foo", OPTARG()));
                break;
            default:  CHECK(false);
        } OPTEND;
    }

    TEST(OPTARG should parse an option with an argument)
    {
        char* args[] = { "prog", "-a", "foo", NULL };
        argc = 3, argv = args;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                CHECK(0 == strcmp("foo", OPTARG()));
                break;
            default:  CHECK(false);
        } OPTEND;
    }

#if 1
    TEST(OPTARG should return null if no argument found for option)
    {
        char* args[] = { "prog", "-a", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                CHECK(0 == OPTARG());
                break;
            default:  CHECK(false);
        } OPTEND;
    }


    TEST(EOPTARG should parse an option with an adjacent argument)
    {
        char* args[] = { "prog", "-afoo", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                CHECK(0 == strcmp("foo", EOPTARG(dummy())));
                break;
            default:  CHECK(false);
        } OPTEND;
    }

    TEST(EOPTARG should parse an option with an argument)
    {
        char* args[] = { "prog", "-a", "foo", NULL };
        argc = 3, argv = args;
        char* arg;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                CHECK(0 == strcmp("foo", EOPTARG(dummy())));
                break;
            default:  CHECK(false);
        } OPTEND;
    }

    TEST(EOPTARG should return abort if no argument found for option)
    {
        char* args[] = { "prog", "-a", NULL };
        argc = 2, argv = args;
        Aborted = false;
        OPTBEGIN {
            case 'a':
                CHECK('a' == OPTC());
                (void)EOPTARG(dummy());
                goto done;
                break;
            default:  CHECK(false);
        } OPTEND;
        done: CHECK(Aborted);
    }
#endif

    //-------------------------------------------------------------------------
    // Miscellaneous Syntax
    //-------------------------------------------------------------------------
#if 1
    TEST(Option processing should terminate at a --)
    {
        char* args[] = { "prog", "--", "-a", NULL };
        argc = 3, argv = args;
        OPTBEGIN {
            default:  CHECK(false);
        } OPTEND;
        CHECK(0 == strcmp("-a", argv[0]));
    }

    TEST(Option processing should recognize long options)
    {
        char* args[] = { "prog", "--foo", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            case '-': CHECK(0 == strcmp("foo", OPTARG())); break;
            default:  CHECK(false);
        } OPTEND;
    }


    TEST(Option processing should recognize - as a positional argument)
    {
        char* args[] = { "prog", "-", NULL };
        argc = 2, argv = args;
        OPTBEGIN {
            default:  CHECK(false);
        } OPTEND;
        CHECK(0 == strcmp("-",argv[0]));
    }
#endif
}
