#ifndef OPT_H
#define OPT_H

extern char* ARGV0;

static inline char* getarg(int* p_argc, char*** p_argv) {
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

/* use main(int argc, char *argv[]) */
#define OPTBEGIN                                                              \
    for (                                                                     \
        ARGV0 = *argv, argc--, argv++;                                        \
        argv[0] && argv[0][1] && argv[0][0] == '-';                           \
        argc--, argv++                                                        \
    ) {                                                                       \
        char argc_ , **argv_, *optarg_;                                       \
        int brk_;                                                             \
        if (argv[0][1] == '-' && !argv[0][2]) {                               \
            argv++, argc--; break;                                            \
        }                                                                     \
        for (brk_=0, argv[0]++, argv_=argv; argv[0][0] && !brk_; argv[0]++) { \
            if (argv_ != argv) break;                                         \
            argc_ = argv[0][0];                                               \
            switch (argc_)

#define OPTEND }}

#define OPTC() (argc_)

#define OPTARG() \
    (optarg_ = getarg(&argc,&argv), brk_ = (optarg_!=0), optarg_)

#define EOPTARG(code) \
    (optarg_ = getarg(&argc,&argv), \
     (!optarg_ ? ((code), abort(), (char*)0) : (brk_ = 1, optarg_)))

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

#define OPTLONG \
    case '-'

#endif
