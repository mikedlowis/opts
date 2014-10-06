#ifndef OPTS_H
#define OPTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

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

#ifdef __cplusplus
}
#endif

#endif
