#ifndef OPTS_H
#define OPTS_H

typedef struct Option_T {
    char* key;
    char* val;
    struct Option_T* next;
} Option_T;

typedef struct OptionList {
    Option_T* head;
    Option_T* tail;
} OptionList_T;

typedef struct Argument_T {
    char* val;
    struct Argument_T* next;
} Argument_T;

typedef struct ArgumentList {
    Argument_T* head;
    Argument_T* tail;
} ArgumentList_T;

typedef struct Result_T {
    ArgumentList_T* arguments;
    OptionList_T* options;
} Result_T;

typedef enum {
    LONG,
    SHORT,
    END
} OptionType_T;

typedef struct OptionConfig_T {
    int type;
    char* name;
    char* dest;
    int has_arg;
    char* desc;
} OptionConfig_T;

typedef struct {
    unsigned int line_idx;
    unsigned int col_idx;
    unsigned int arg_count;
    char** arg_vect;
    char current;
    OptionConfig_T* options;
    Result_T* results;
} StreamContext_T;

Result_T* OPTS_ParseOptions( OptionConfig_T* opts, int argc, char** argv );

void OPTS_InitContext( StreamContext_T* ctx, int argc, char** argv );

void OPTS_ParseShortOption( StreamContext_T* ctx );

void OPTS_ParseLongOption( StreamContext_T* ctx );

void OPTS_ParseArgument( StreamContext_T* ctx );

char* OPTS_NextToken( StreamContext_T* ctx );

char OPTS_NextCharacter( StreamContext_T* ctx );

char* OPTS_AppendCharacter( char* str, char ch );

#endif
