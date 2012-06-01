#ifndef OPTS_H
#define OPTS_H

typedef struct {
    int line_idx;
    int col_idx;
    int arg_count;
    char** arg_vect;
} StreamContext_T;

typedef struct {
    StreamContext_T stream;
    char* cur_token;
} TokenContext_T;

typedef struct OptionList {
    char** key;
    char** val;
    struct OptionList* next;
} OptionList_T;

typedef struct {
    TokenContext_T tok_stream;
    OptionList_T* options;
} ParserContext_T;

OptionList_T* OPTS_ParseOptions( int argc, char** argv );

OptionList_T* OPTS_ParseOption( ParserContext_T* ctx );

char* OPTS_NextToken( TokenContext_T* ctx );

char OPTS_NextCharacter( StreamContext_T* ctx );

void OPTS_FreeOptionList( OptionList_T* options );

#endif
