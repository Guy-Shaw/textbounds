// eprint.h

#ifdef MAIN

FILE *errprint_fh;

#else

extern FILE *errprint_fh;

#endif

#define eprint(str) fputs((str), errprint_fh)
#define eputchar(c) putc((c), errprint_fh)

#if defined(__GNUC__)

#define eprintf(fmt, ...) \
    fprintf(errprint_fh, (fmt), ## __VA_ARGS__)

#else

extern int eprintf(const char *, ...);

#endif /* __GNUC__ */
