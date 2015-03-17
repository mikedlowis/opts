#ifndef OPTS_H
#define OPTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/** Structure representing an option to be parsed */
typedef struct {
    /** The name of the option as it will appear on the commandline */
    char* name;
    /** Flag indicating whether the option expects an argument or not */
    bool has_arg;
    /** An optional tag which can be used to group related options together
     *  logically */
    char* tag;
    /** A short description of the flag used for displaying help messages */
    char* desc;
} opts_cfg_t;

/**
 * Parse the command line options using the provided option definition list.
 *
 * @param opts Pointer to a list of option definitions
 * @param argc The number of arguments in the vector
 * @param argv The vector of command line arguments
 */
void opts_parse(opts_cfg_t* opts, int argc, char** argv);

/**
 * Resets the global state back to defaults. This includes freeing any
 * allocated memory and clearing any saved pointers to NULL.
 */
void opts_reset(void);

/**
 * Determines if a given option is set. This function searches the list of
 * parsed options for an entry with the given name and/or the given tag. A value
 * of NULL for either parameter will match everything.
 *
 * @param name The name of the option to find.
 * @param tag  The tag of the option to find.
 *
 * @return true if an entry was found, false otherwise.
 */
bool opts_is_set(const char* name, const char* tag);

/**
 * Searches for the last received option with the given name and/or tag and
 * does a string comparison of it's value. The parsed options are stored
 * internally in reverse order so the first match equates to the last option
 * that the parser saw on the command line. The value returned is the text of
 * any argument that was received for the option or the name of the option
 * itself if the option does not take an argument.
 *
 * @param name  The name of the option to find.
 * @param tag   The tag of the option to find.
 * @param value The value to compare.
 *
 * @return true if the value matches exactly, false otherwise.
 */
bool opts_equal(const char* name, const char* tag, const char* value);

/**
 * Search for a parsed option value with the given name and/or tag. If multiple
 * matches are found, only the first match is found. The parsed options are
 * stored internally in reverse order so the first match equates to the last
 * option that the parser saw on the command line. The value returned is the
 * text of any argument that was received for the option or the name of the
 * option itself if the option does not take an argument.
 *
 * @param name The name of the option to search for.
 * @param tag  The tag of the option to search for.
 *
 * @return The text of the value of the parsed option.
 */
const char* opts_get_value(const char* name, const char* tag);

/**
 * Search for a group of parsed option values with the given name and/or tag.
 * The value returned for each matching option is the text of the argument that
 * was received for the option. If the option does not expect an argument then
 * the value is returned as the name of the option itself.
 *
 * @param name The name of the options to search for.
 * @param tag  The tag of the options to search for.
 *
 * @return Pointer to the array of options.
 */
const char** opts_select(const char* name, const char* tag);

/**
 * Returns a null terminated array of strings representing the arguments of the
 * executable. These are the entries provided on the command line that are not
 * parsed as options. A common example would be the list of files passed to the
 * unix "cat" command.
 *
 * @return Pointer to the array of arguments.
 */
const char** opts_arguments(void);

/**
 * Returns the program name as received on the command line.
 *
 * @return String representing the program name.
 */
const char* opts_prog_name(void);

/**
 * Prints out the options and their descriptions in a tabular format to the
 * given file handle.
 *
 * @param ofile The file handle to use for output.
 * @param opts  The list of option definitions.
 */
void opts_print_help(FILE* ofile, opts_cfg_t* opts);

#ifdef __cplusplus
}
#endif

#endif
