#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opts.h"

typedef struct {
    char* name;
    char* tag;
    char* value;
} option_t;

typedef struct entry_t {
    void* value;
    struct entry_t* next;
} entry_t;

typedef enum { LONG, SHORT } OptionType_T;

typedef struct {
    unsigned int line_idx;
    unsigned int col_idx;
    unsigned int arg_count;
    char** arg_vect;
    int current;
    opts_cfg_t* options;
} StreamContext_T;

static entry_t* Options   = NULL;
static entry_t* Arguments = NULL;

static void opts_parse_short_option( StreamContext_T* ctx );
static void opts_parse_long_option( StreamContext_T* ctx );
static char* opts_parse_optarg(StreamContext_T* ctx, char* opt_name);
static void opts_parse_argument( StreamContext_T* ctx );
static void opts_parse_error(const char* msg, char* opt_name);

static opts_cfg_t* opts_get_option_config( opts_cfg_t* opts, OptionType_T typ, char* name );
static char* opts_next_token( StreamContext_T* ctx );
static void opts_consume_ws( StreamContext_T* ctx );
static char opts_next_char( StreamContext_T* ctx );
static char* opts_append_char( char* str, char ch );
static char* strclone(const char* p_old);

static void opts_add_option(char* name, char* tag, char* arg);
static void opts_add_argument(char* arg);

void opts_parse( opts_cfg_t* opts, int argc, char** argv ) {
    /* Setup the stream */
    StreamContext_T ctx;
    ctx.line_idx  = 0;
    ctx.col_idx   = -1;
    ctx.arg_count = argc;
    ctx.arg_vect  = argv;
    ctx.options   = opts;
    (void)opts_next_char( &ctx ); /* Loads up the first char */

    /* Until we run out of characters */
    while (ctx.current != EOF) {
        opts_consume_ws( &ctx );
        /* If we have an option */
        if ('-' == ctx.current) {
            /* And it's a long one */
            if ('-' == opts_next_char( &ctx )) {
                /* Consume the second '-' */
                opts_next_char( &ctx );
                /* Parse the option */
                opts_parse_long_option( &ctx );
            } else {
                /* Parse the option */
                opts_parse_short_option( &ctx );
            }
        } else {
            opts_parse_argument( &ctx );
        }
    }
}

void opts_reset(void) {
    while (Options != NULL) {
        entry_t* entry = Options;
        option_t* opt  = (option_t*)entry->value;
        Options = entry->next;
        free(opt->name);
        free(opt->tag);
        if(opt->name != opt->value)
            free(opt->value);
        free(opt);
        free(entry);
    }

    while (Arguments != NULL) {
        entry_t* entry = Arguments;
        char* arg  = (char*)entry->value;
        Arguments = entry->next;
        free(arg);
        free(entry);
    }
}

static void opts_parse_short_option( StreamContext_T* ctx ) {
    char opt[2] = { ctx->current, '\0' };
    char* opt_name = strclone(opt);
    opts_cfg_t* config = opts_get_option_config( ctx->options, SHORT, opt_name );
    if (config != NULL) {
        char* opt_arg = NULL;
        (void)opts_next_char( ctx );
        if (config->has_arg)
            opt_arg = opts_parse_optarg( ctx, opt_name );
        else if ((' ' != ctx->current) && (EOF != ctx->current))
            opts_parse_short_option( ctx );
        opts_add_option( opt_name, config->tag, opt_arg );
    } else {
        opts_parse_error("Unknown Option", opt_name);
    }
}

static void opts_parse_long_option( StreamContext_T* ctx ) {
    char* opt_name = opts_next_token( ctx );
    opts_cfg_t* config = opts_get_option_config( ctx->options, LONG, opt_name );
    if (config != NULL) {
        char* opt_arg = NULL;
        if (config->has_arg)
            opt_arg = opts_parse_optarg( ctx, opt_name );
        opts_add_option( opt_name, config->tag, opt_arg );
    } else {
        opts_parse_error("Unknown Option", opt_name);
    }
}

static char* opts_parse_optarg(StreamContext_T* ctx, char* opt_name) {
    opts_consume_ws( ctx );
    if (('-' == ctx->current) || (EOF == ctx->current))
        opts_parse_error("Expected an argument, none received", opt_name);
    return opts_next_token( ctx );
}

static void opts_parse_error(const char* msg, char* opt_name) {
    fprintf(stderr, "Option '%s' : %s\n", opt_name, msg);
    free(opt_name);
    exit(1);
}

static void opts_parse_argument( StreamContext_T* ctx ) {
    char* arg_val = opts_next_token( ctx );
    if (NULL != arg_val)
        opts_add_argument(arg_val);
}

static opts_cfg_t* opts_get_option_config( opts_cfg_t* opts, OptionType_T type, char* name ) {
    opts_cfg_t* cfg = NULL;
    int i = 0;
    while( opts[i].name != NULL ) {
        OptionType_T curr_type = (strlen(opts[i].name) > 1) ? LONG : SHORT;
        if ((curr_type == type) && (0 == strcmp(opts[i].name, name))) {
            cfg = &(opts[i]);
            break;
        }
        i++;
    }
    return cfg;
}

static char* opts_next_token( StreamContext_T* ctx ) {
    char* tok = NULL;

    opts_consume_ws( ctx );
    if (EOF != ctx->current) {
        // Setup the string
        tok = (char*)malloc(2);
        tok[0] = ctx->current;
        tok[1] = '\0';

        (void)opts_next_char( ctx );
        while ((EOF != ctx->current) && (' ' != ctx->current)) {
            tok = opts_append_char( tok, ctx->current );
            (void)opts_next_char( ctx );
        }
    }
    return tok;
}

static void opts_consume_ws( StreamContext_T* ctx ) {
    while (' ' == ctx->current)
        (void)opts_next_char( ctx );
}

static char opts_next_char( StreamContext_T* ctx ) {
    char current = EOF;
    ctx->current = EOF;

    if (ctx->line_idx < ctx->arg_count) {
        ctx->col_idx++;
        if (ctx->line_idx < ctx->arg_count) {
            char temp = ctx->arg_vect[ ctx->line_idx ][ ctx->col_idx ];
            if (temp == '\0') {
                ctx->col_idx = -1;
                ctx->line_idx++;
                current = ' ';
                ctx->current = current;
            } else {
                temp = (temp == '=') ? ' ' : temp;
                current = temp;
                ctx->current = current;
            }
        }
    }

    return current;
}

static char* opts_append_char( char* str, char ch ) {
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

/*****************************************************************************/

static void opts_add_option(char* name, char* tag, char* arg) {
    option_t* option = (option_t*)malloc(sizeof(option_t));
    option->name     = name;
    option->tag      = strclone(tag);
    option->value    = (NULL == arg) ? name : arg;
    entry_t* entry   = (entry_t*)malloc(sizeof(entry_t));
    entry->value     = (void*)option;
    entry->next      = Options;
    Options          = entry;
}

static void opts_add_argument(char* arg_val) {
    entry_t* entry = (entry_t*)malloc(sizeof(entry_t));
    entry->value   = (void*)arg_val;
    entry->next    = Arguments;
    Arguments      = entry;
}

/*****************************************************************************/

option_t* find_option(const char* name, const char* tag) {
    option_t* p_opt = NULL;
    entry_t* current = Options;
    while (current != NULL) {
        option_t* curr_opt = (option_t*)current->value;
        if (((NULL == name) || (0 == strcmp(name, curr_opt->name))) &&
            ((NULL == tag)  || (0 == strcmp(tag,  curr_opt->tag)))) {
            p_opt = curr_opt;
            break;
        }
        current = current->next;
    }
    return p_opt;
}

bool opts_is_set(const char* name, const char* tag) {
    return (NULL != find_option(name,tag));
}

const char* opts_get_value(const char* name, const char* tag) {
    option_t* p_opt = find_option(name,tag);
    return (NULL == p_opt) ? NULL : p_opt->value;
}

bool opts_equal(const char* name, const char* tag, const char* value) {
    return (0 == strcmp(value, opts_get_value(name,tag)));
}

const char** opts_select(const char* name, const char* tag) {
    size_t index = 0;
    const char** ret = (const char**)malloc(sizeof(const char*));
    ret[index] = NULL;

    entry_t* current = Options;
    while (current != NULL) {
        option_t* curr_opt = (option_t*)current->value;
        if (((NULL == name) || (0 == strcmp(name, curr_opt->name))) &&
            ((NULL == tag)  || (0 == strcmp(tag,  curr_opt->tag)))) {
            ret = (const char**)realloc(ret, (index+2)*sizeof(const char*));
            ret[index++] = curr_opt->value;
            ret[index]   = NULL;
        }
        current = current->next;
    }

    return ret;
}

const char** opts_arguments(void) {
    size_t index = 0;
    const char** ret = (const char**)malloc(sizeof(const char*));
    entry_t* entry = Arguments;
    while (NULL != entry) {
        ret = (const char**)realloc(ret, (index+2)*sizeof(const char*));
        ret[index++] = (const char*)entry->value;
        ret[index]   = NULL;
        entry = entry->next;
    }
    return ret;
}


