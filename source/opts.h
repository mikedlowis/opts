#ifndef OPTS_H
#define OPTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    char* name;
    bool has_arg;
    char* tag;
    char* desc;
} opts_cfg_t;

void opts_parse(opts_cfg_t* opts, int argc, char** argv);

void opts_reset(void);

bool opts_is_set(const char* name, const char* tag);

bool opts_equal(const char* name, const char* tag, const char* value);

const char* opts_get_value(const char* name, const char* tag);

const char** opts_select(const char* name, const char* tag);

const char** opts_arguments(void);

const char* opts_prog_name(void);

void opts_print_help(FILE* ofile, opts_cfg_t* opts);

#ifdef __cplusplus
}
#endif

#endif
