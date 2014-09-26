#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opts.h"

static char* strclone(const char* p_old);

Result_T* OPTS_ParseOptions( OptionConfig_T* opts, int argc, char** argv )
{
    // Setup the stream
    StreamContext_T ctx;
    ctx.options = opts;
    OPTS_InitContext( &ctx, argc, argv );

    // Until we run out of characters
    while( ctx.current != EOF )
    {
        OPTS_ConsumeWhitespace( &ctx );

        // If we have an option
        if ( '-' == ctx.current )
        {
            // And it's a long one
            if( '-' == OPTS_NextCharacter( &ctx ) )
            {
                // Consume the second '-'
                OPTS_NextCharacter( &ctx );

                // Parse the option
                OPTS_ParseLongOption( &ctx );
            }
            // Or a short one
            else
            {
                // Parse the option
                OPTS_ParseShortOption( &ctx );
            }
        }
        // Or we have a floating argument
        else
        {
            OPTS_ParseArgument( &ctx );
        }
    }

    return ctx.results;
}

void OPTS_InitContext( StreamContext_T* ctx, int argc, char** argv )
{
    // Setup the char stream
    ctx->line_idx  = 0;
    ctx->col_idx   = -1;
    ctx->arg_count = argc;
    ctx->arg_vect  = argv;
    (void)OPTS_NextCharacter( ctx ); // Loads up the first char

    // Setup the results object
    ctx->results = (Result_T*)malloc( sizeof( Result_T ) );
    ctx->results->options = (OptionList_T*)malloc( sizeof( OptionList_T ) );
    ctx->results->options->head = NULL;
    ctx->results->options->tail = NULL;
    ctx->results->arguments = (ArgumentList_T*)malloc( sizeof( ArgumentList_T ) );
    ctx->results->arguments->head = NULL;
    ctx->results->arguments->tail = NULL;
}

void OPTS_ParseShortOption( StreamContext_T* ctx )
{
    // Get Config
    char opt_name[2] = { ctx->current, '\0' };
    OptionConfig_T* config = OPTS_GetOptConfig( ctx->options, SHORT, opt_name );
    if( config != NULL )
    {
        char* opt_arg = NULL;
        (void)OPTS_NextCharacter( ctx );
        if( 1 == config->has_arg )
        {
            OPTS_ConsumeWhitespace( ctx );
            if( ('-' == ctx->current) || (EOF  == ctx->current) )
            {
                printf("Expected an argument for option '%s'\n", opt_name);
                exit(1);
            }
            opt_arg = OPTS_NextToken( ctx );
        }
        else if( (' ' != ctx->current) && (EOF != ctx->current) )
        {
            OPTS_ParseShortOption( ctx );
        }
        OPTS_AddOption( ctx->results, opt_name, opt_arg );
    }
    else
    {
        printf("Unknown Option '%s'\n", opt_name);
        exit(1);
    }
}

void OPTS_ParseLongOption( StreamContext_T* ctx )
{
    char* opt_name = OPTS_NextToken( ctx );
    OptionConfig_T* config = OPTS_GetOptConfig( ctx->options, LONG, opt_name );
    if( config != NULL )
    {
        char* opt_arg = NULL;
        if( 1 == config->has_arg )
        {
            OPTS_ConsumeWhitespace( ctx );
            if( ('-' == ctx->current) || (EOF == ctx->current) )
            {
                printf("Expected an argument for option '%s'\n", opt_name);
                exit(1);
            }
            opt_arg = OPTS_NextToken( ctx );
        }
        OPTS_AddOption( ctx->results, opt_name, opt_arg );
    }
    else
    {
        printf("Unknown Option '%s'\n", opt_name);
        exit(1);
    }
}

void OPTS_ParseArgument( StreamContext_T* ctx )
{
    char* arg_val = OPTS_NextToken( ctx );
    if( NULL != arg_val )
    {
        Argument_T* arg = (Argument_T*)malloc( sizeof( Argument_T ) );
        arg->val  = arg_val;
        arg->next = NULL;
        if( ctx->results->arguments->head == NULL )
        {
            ctx->results->arguments->head = arg;
            ctx->results->arguments->tail = arg;
        }
        else
        {
            ctx->results->arguments->tail->next = arg;
            ctx->results->arguments->tail = arg;
        }
    }
}

OptionConfig_T* OPTS_GetOptConfig( OptionConfig_T* opts, OptionType_T typ, char* name )
{
    OptionConfig_T* cfg = NULL;
    int i = 0;
    while( opts[i].type != END )
    {
        if( (opts[i].type == typ) && (0 == strcmp(opts[i].name, name)) )
        {
            cfg = &(opts[i]);
            break;
        }
        i++;
    }
    return cfg;
}

char* OPTS_NextToken( StreamContext_T* ctx )
{
    char* tok = NULL;

    OPTS_ConsumeWhitespace( ctx );
    if ( EOF != ctx->current )
    {
        // Setup the string
        tok = (char*)malloc(2);
        tok[0] = ctx->current;
        tok[1] = '\0';

        (void)OPTS_NextCharacter( ctx );
        while( (EOF != ctx->current) && (' ' != ctx->current) )
        {
            tok = OPTS_AppendCharacter( tok, ctx->current );
            (void)OPTS_NextCharacter( ctx );
        }
    }
    return tok;
}

void OPTS_AddOption( Result_T* res, char* name, char* arg )
{
    if( (NULL != res) && (NULL != res->options) )
    {
        Option_T* opt = (Option_T*)malloc( sizeof( Option_T ) );
        opt->key = strclone( name );
        opt->val = arg;
        opt->next = NULL;
        if( res->options->head == NULL )
        {
            res->options->head = opt;
            res->options->tail = opt;
        }
        else
        {
            res->options->tail->next = opt;
            res->options->tail = opt;
        }
    }
}

void OPTS_ConsumeWhitespace( StreamContext_T* ctx )
{
    while( ' ' == ctx->current )
    {
        (void)OPTS_NextCharacter( ctx );
    }
}

#include <stdio.h>
char OPTS_NextCharacter( StreamContext_T* ctx )
{
    char current = EOF;
    ctx->current = EOF;

    if( ctx->line_idx < ctx->arg_count )
    {
        ctx->col_idx++;

        if( ctx->line_idx < ctx->arg_count )
        {
            char temp = ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ];
            if( temp == '\0' )
            {
                ctx->col_idx = -1;
                ctx->line_idx++;
                current = ' ';
                ctx->current = current;
            }
            else
            {
                temp = (temp == '=') ? ' ' : temp;
                current = temp;
                ctx->current = current;
            }
        }
    }
    return current;
}

char* OPTS_AppendCharacter( char* str, char ch )
{
    unsigned int new_size = strlen( str ) + 2;
    str = (char*)realloc( str, new_size );
    str[ new_size - 2 ] = ch;
    str[ new_size - 1 ] = '\0';
    return str;
}


static char* strclone(const char* p_old) {
    size_t length = strlen(p_old);
    char* p_str = (char*)malloc(length+1);
    memcpy(p_str, p_old, length);
    p_str[length] = '\0';
    return p_str;
}

