/**
 * @file
 * @author Mike D. Lowis
 *
 * This project is an implementation of a very lightweight command-line options
 * parser. It is capable of interpreting the Unix/GNU style command line
 * argument flags. It is designed to be platform independent and can be used as
 * a static library or directly as source.
 *
 * The library is designed to work like something of a database for your
 * command line. At initialization time you feed in the arguments vector (your
 * data records) and a list of option definitions (your schema). The library
 * then parses the options and stores them internally for querying later.
 * Several querying functions are provided that cover the most common use cases
 * for options handling.
 *
 * All of the query functions take an option name and an option tag as
 * parameters. These two arguments will be used to search the list of parsed
 * options for any that match both parameters. In some cases only one of the
 * parameters is needed for a query. To facilitate this, the query functions
 * will interpret a NULL pointer to match everything. An example of this would
 * be selecting all parsed options with the tag "foo":
 *
 * const char** foo_lst = opts_select(NULL,"foo");
 *
 * In the example above, the NULL would match *all* parsed options and from that
 * group only those having the tag "foo" would be returned. Another common
 * scenario would be searching by parameter name:
 *
 * const char** includes = opts_select("I", NULL);
 *
 * In this scenario all of the options that were parsed having the name 'I'
 * would be returned regardless of their tag value. This would equate to all of
 * the instances of '-I' that would occur on the command line for an invocation
 * of gcc.
 *
 * With this design you should be able to let the library handle your options
 * parsing while you focus on your application logic using appropriate queries
 * to change behavior where necessary.
 *
 */
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
    /** The name of the option as it will appear on the command line. If the
     *  name is a single character in length the parse will treat it as a short
     *  option and require a single dash. Otherwise, it is considered a long
     *  option and will require double dashes */
    char* name;
    /** Flag indicating whether the option expects an argument or not */
    bool has_arg;
    /** An optional tag which can be used to group related options together
     *  logically */
    char* tag;
    /** A short description of the flag used for displaying help messages */
    char* desc;
} opts_cfg_t;

typedef void (*opts_err_cbfn_t)(const char* msg, char* opt_name);

/**
 * Parse the command line options using the provided option definition list.
 *
 * @param opts Pointer to a list of option definitions
 * @param argc The number of arguments in the vector
 * @param argv The vector of command line arguments
 */
void opts_parse(opts_cfg_t* opts, opts_err_cbfn_t err_cb, int argc, char** argv);

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
