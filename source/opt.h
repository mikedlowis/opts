/* This file implements a simple POSIX-style option parsing implementation as
 * a set of macros. The file is heavily influenced and inspired by the arg.h
 * file from suckless.org (http://git.suckless.org/libsl/tree/arg.h). That file
 * is in turn inspired by the corresponding macros defined in plan9.
 *
 * The interface in this file assumes that the main function will have the
 * following prototype:
 *
 * int main(int argc, char** argv);
 *
 * An example usage of the interface would look something like the follwoing:
 *
 * char* ARGV0;
 * int main(int argc, char** argv) {
 *     OPTBEGIN {
 *         case 'a': printf("Simple option\n"); break;
 *         case 'b': printf("Option with arg: %s\n", OPTARG()); break;
 *         default:  printf("Unknown option!\n");
 *     } OPTEND;
 *     return 0;
 * }
 */
#ifndef OPT_H
#define OPT_H

/* This variable contains the value of argv[0] so that it can be referenced
 * again once the option parsing is done. This variable must be defined by the
 * program.
 *
 * NOTE: Ensure that you define this variable with external linkage (i.e. not
 * static)
 */
extern char* ARGV0;

/* This is a helper function used by the macros in this file to parse the next
 * option from the command line.
 */
static inline char* getopt(int* p_argc, char*** p_argv) {
    if (!(*p_argv)[0][1] && !(*p_argv)[1]) {
        return (char*)0;
    } else if ((*p_argv)[0][1]) {
        return &(*p_argv)[0][1];
    } else {
        *p_argv = *p_argv + 1;
        *p_argc = *p_argc - 1;
        return (*p_argv)[0];
    }
}

/* This macro is almost identical to the ARGBEGIN macro from suckless.org. If
 * it ain't broke, don't fix it. */
#define OPTBEGIN                                                              \
    for (                                                                     \
        ARGV0 = *argv, argc--, argv++;                                        \
        argv[0] && argv[0][1] && argv[0][0] == '-';                           \
        argc--, argv++                                                        \
    ) {                                                                       \
        int brk_; char argc_ , **argv_, *optarg_;                             \
        if (argv[0][1] == '-' && !argv[0][2]) {                               \
            argv++, argc--; break;                                            \
        }                                                                     \
        for (brk_=0, argv[0]++, argv_=argv; argv[0][0] && !brk_; argv[0]++) { \
            if (argv_ != argv) break;                                         \
            argc_ = argv[0][0];                                               \
            switch (argc_)

/* Terminate the option parsing. */
#define OPTEND }}

/* Get the current option chracter */
#define OPTC() (argc_)

/* Get an argument from the command line and return it as a string. If no
 * argument is available, this macro returns NULL */
#define OPTARG() \
    (optarg_ = getopt(&argc,&argv), brk_ = (optarg_!=0), optarg_)

/* Get an argument from the command line and return it as a string. If no
 * argument is available, this macro executes the provided code. If that code
 * returns, then abort is called. */
#define EOPTARG(code) \
    (optarg_ = getopt(&argc,&argv), \
     (!optarg_ ? ((code), abort(), (char*)0) : (brk_ = 1, optarg_)))

/* Helper macro to recognize number options */
#define OPTNUM \
    case '0':  \
    case '1':  \
    case '2':  \
    case '3':  \
    case '4':  \
    case '5':  \
    case '6':  \
    case '7':  \
    case '8':  \
    case '9'

/* Helper macro to recognize "long" options ala GNU style. */
#define OPTLONG \
    case '-'

#endif
