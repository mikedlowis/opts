#include <stdlib.h>
#include <stdio.h>
#include "opts.h"

OptionList_T* OPTS_ParseOptions( int argc, char** argv )
{
    ParserContext_T ctx;
    ctx.options = 0;

    //while( ctx.tok_stream.stream.cur_char != EOF )
    //{
    //    OPTS_ParseOption( &(ctx) );
    //}

    return ctx.options;
}

OptionList_T* OPTS_ParseOption( ParserContext_T* ctx )
{
    return NULL;
}

char* OPTS_NextToken( TokenContext_T* ctx )
{
    return NULL;
}

char OPTS_NextCharacter( StreamContext_T* ctx )
{
    char current = EOF;

    if( ctx->line_idx < ctx->arg_count )
    {
        ctx->col_idx++;

        if(ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ] == '\0')
        {
            ctx->col_idx = 0;
            ctx->line_idx++;
        }

        if( ctx->line_idx < ctx->arg_count )
        {
            current = ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ];
        }
    }

    return current;
}

void OPTS_FreeOptionList( OptionList_T* opts )
{

}

