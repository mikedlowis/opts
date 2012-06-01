#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opts.h"

Result_T* OPTS_ParseOptions( OptionConfig_T* opts, int argc, char** argv )
{
    // Setup the stream
    StreamContext_T ctx;
    ctx.options = opts;
    OPTS_InitContext( &ctx, argc, argv );

    // Until we run out of characters
    while( ctx.current != EOF )
    {
        // Get rid of any whitespace
        while ( ' ' == ctx.current )
        {
            (void)OPTS_NextCharacter( &ctx );
        }

        // If we have an option
        if ( '-' == ctx.current )
        {
            // And its a long one
            if( '-' == OPTS_NextCharacter( &ctx ) )
            {
                (void)OPTS_ParseLongOption( &ctx );
            }
            // or a short one
            else
            {
                (void)OPTS_ParseShortOption( &ctx );
            }
        }
        // Or we have a floating argument
        else
        {
            (void)OPTS_ParseArgument( &ctx );
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
}

void OPTS_ParseLongOption( StreamContext_T* ctx )
{
}

void OPTS_ParseArgument( StreamContext_T* ctx )
{
}

char* OPTS_NextToken( StreamContext_T* ctx )
{
    char* tok = NULL;
    if ( EOF != ctx->current )
    {
        // Setup the string
        tok = (char*)malloc(2);
        tok[0] = ctx->current;
        tok[1] = '\0';

        (void)OPTS_NextCharacter( ctx );
        while( (EOF != ctx->current) && (' ' != ctx->current) && ('=' != ctx->current) )
        {
            OPTS_AppendCharacter( tok, ctx->current );
            (void)OPTS_NextCharacter( ctx );
        }
    }
    return tok;
}

char OPTS_NextCharacter( StreamContext_T* ctx )
{
    char current = EOF;
    ctx->current = EOF;

    if( ctx->line_idx < ctx->arg_count )
    {
        ctx->col_idx++;

        if( ctx->line_idx < ctx->arg_count )
        {
            if(ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ] == '\0')
            {
                ctx->col_idx = -1;
                ctx->line_idx++;
                current = ' ';
                ctx->current = current;
            }
            else
            {
                current = ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ];
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

