/*
 * Filename: textbounds-argv.c
 * Library: libtextbounds
 * Brief: Run textbounds on a list of files.  Top-level (main) program
 *
 * Copyright (C) 2019 Guy Shaw
 * Written by Guy Shaw <gshaw@acm.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE 1

#include <stdlib.h>         // Import exit()
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>          // Import isprint()
// #include <sys/wait.h>

#include <textbounds.h>

#define IMPORT_FVH
#include <cscript.h>

#include <getopt_int.h>

#include <unistd.h>         // Import isatty()

typedef unsigned int uit_t;

extern void text_bounds(textbox_t *);

struct cmd {
    // Specifications
    int argc;
    char **argv;
    const char *cmd_path;
    const char *cmd_name;

    // Options
    bool verbose;
    bool debug;

    // Status
    int   ioerr;
};

typedef struct cmd cmd_t;

static cmd_t cmdbuf;
static cmd_t *cmd = &cmdbuf;

bool verbose  = false;
bool debug    = false;

static bool opt_show_argv = false;

FILE *errprint_fh = NULL;
FILE *dbgprint_fh = NULL;

extern char *program_path;
extern char *program_name;

#define OPT_BASE       0xf000
#define OPT_NAME       0x0001
#define OPT_LINES      0x0002
#define OPT_COLUMNS    0x0004
#define OPT_FORMAT     0x0100
#define OPT_SHOW_ARGV  0x0200

static char *result_format = NULL;
static uint_t fmt_options = 0;

static struct option long_options[] = {
    {"help",              no_argument,       0,  'h'},
    {"version",           no_argument,       0,  'V'},
    {"verbose",           no_argument,       0,  'v'},
    {"debug",             no_argument,       0,  'd'},
    {"name",              no_argument,       0,  OPT_BASE | OPT_NAME},
    {"lines",             no_argument,       0,  OPT_BASE | OPT_LINES},
    {"columns",           no_argument,       0,  OPT_BASE | OPT_COLUMNS},
    {"format",            required_argument, 0,  OPT_BASE | OPT_FORMAT},
    {0, 0, 0, 0 }
};

static const char usage_text[] =
    "Options:\n"
    "  --help|-h|-?      Show this help message and exit\n"
    "  --version         Show textbounds version information and exit\n"
    "  --verbose|-v      verbose\n"
    "  --debug|-d        debug\n"
    "  --name            Show file name\n"
    "  --lines           Show number of lines (same as wc -l)\n"
    "  --columns         Show number of columns (maximum line length)\n"
    ;

static const char version_text[] =
    "0.1\n"
    ;

static const char copyright_text[] =
    "Copyright (C) 2019 Guy Shaw\n"
    "Written by Guy Shaw\n"
    ;

static const char license_text[] =
    "License GPLv3+: GNU GPL version 3 or later"
    " <http://gnu.org/licenses/gpl.html>.\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.\n"
    ;

static void
fshow_version(FILE *f)
{
    fputs(version_text, f);
    fputc('\n', f);
    fputs(copyright_text, f);
    fputc('\n', f);
    fputs(license_text, f);
    fputc('\n', f);
}

static void
show_version(void)
{
    fshow_version(stdout);
}

static void
usage(void)
{
    eprintf("usage: %s [ <options> ]\n", program_name);
    eprint(usage_text);
}

static inline bool
is_long_option(const char *s)
{
    return (s[0] == '-' && s[1] == '-');
}

static inline char *
vischar_r(char *buf, size_t sz, int c)
{
    if (isprint(c)) {
        buf[0] = c;
        buf[1] = '\0';
    }
    else {
        snprintf(buf, sz, "\\x%02x", c);
    }
    return (buf);
}

/*
 * Options that specify how text bounds are to be formatted and displayed.
 *
 *   --lines    lines  (height in lines; same as wc -l)
 *   --columns  (maximum line length)
 *   --name     show file name
 *
 */


/*
 * const char *fmt, size_t Lines, size_t MaxWidth, uint_t fmt_options)
 * fshow_textbounds(FILE *f, const char *fname, ...
 * const char *fmt, size_t Lines, size_t MaxWidth, uint_t fmt_options)
 */

static void
fshow_textbounds(FILE *f, const char *fname, textbox_t *txt)
{
    char fmtbuf[32];
    char *fmt = txt->fmt;
    uint_t fmt_options = txt->fmt_options;

    /*
     * If a format string is given, use it;
     * otherwise, build our own format string based on options
     * (--name --lines --columns).
     *
     */
    if (fmt == NULL && fmt_options == 0) {
        fmt = "%cx%l";
    }
    else if (fmt == NULL) {
        char *bp;

        bp = fmtbuf;
        if ((fmt_options & OPT_NAME) != 0) {
            if (bp > fmtbuf) {
                *bp++ = ' ';
            }
            *bp++ = '%';
            *bp++ = 'f';
        }

        if ((fmt_options & OPT_LINES) != 0) {
            if (bp > fmtbuf) {
                *bp++ = ' ';
            }
            *bp++ = '%';
            *bp++ = 'l';
        }

        if ((fmt_options & OPT_COLUMNS) != 0) {
            if (bp > fmtbuf) {
                *bp++ = ' ';
            }
            *bp++ = '%';
            *bp++ = 'c';
        }
        *bp = '\0';
        fmt = fmtbuf;
    }


    dbg_printf("fname=[%s], fmt=[%s], lines=%zu, columns=%zu, fmt_options=%u\n",
            fname, fmt, txt->lines, txt->columns, fmt_options);

    /*
     * Interpret format string.
     * Intercalate textbounds properties.
     */
    const char *fp;
    for (fp = fmt; *fp; ++fp) {
        int c;

        c = *fp;
        switch (c) {
            default:
                fputc(c, f);
                break;
            case '%':
                ++fp;
                c = *fp;
                switch (c) {
                    default:
                        fprintf(f, "{%%%c=ERROR}", c);
                        break;
                    case '%':
                        fputc(c, f);
                        break;
                    case 'f':
                        fprintf(f, "%s", fname);
                        break;
                    case 'l':
                        fprintf(f, "%zu", txt->lines);
                        break;
                    case 'c':
                        fprintf(f, "%zu", txt->columns);
                        break;
                }
                break;
        }
    }
    fputc('\n', f);
}

static struct _getopt_data null_getopts_data;

void
getopts_init(struct _getopt_data *ctx)
{
    *ctx = null_getopts_data;
}

/*
 * Arguments come from the command line of @command{textbounds} itself.
 */
int
textbounds_getopt(cmd_t *cmd, int argc, char **argv, bool setargv)
{
    struct _getopt_data getopt_ctx;
    extern char *optarg;
    extern int optind, opterr, optopt;
    int option_index;
    int err_count;
    int optc;
    int rv;

    getopts_init(&getopt_ctx);
    option_index = 0;
    err_count = 0;
    optind = 1;
    opterr = 0;

    while (true) {
        int this_option_optind;

        if (err_count > 10) {
            eprintf("%s: Too many option errors.\n", program_name);
            break;
        }

        this_option_optind = optind ? optind : 1;
        getopt_ctx.optind = optind;
        getopt_ctx.opterr = opterr;
        optc = cs_getopt_internal_r(argc, argv, "+hVdvc", long_options, &option_index, 0, &getopt_ctx, 0);

        optind = getopt_ctx.optind;
        optarg = getopt_ctx.optarg;
        optopt = getopt_ctx.optopt;

        if (optc == -1) {
            break;
        }

        if (debug) {
            dbg_printf("optc=0x%x", optc);
            if (optc >= 0 && optc <= 127 && isprint(optc)) {
                dbg_printf("='%c'", optc);
            }
            eprintf("\n");
        }

        rv = 0;
        if (optc == '?' && optopt == '?') {
            optc = 'h';
        }

        switch (optc) {
        case 'V':
            show_version();
            exit(0);
            break;
        case 'h':
            fputs(usage_text, stdout);
            exit(0);
            break;
        case 'd':
            debug = true;
            break;
        case 'v':
            verbose = true;
            break;
        case OPT_BASE|OPT_LINES:
            fmt_options |= OPT_LINES;
            break;
        case OPT_BASE|OPT_COLUMNS:
            fmt_options |= OPT_COLUMNS;
            break;
        case OPT_BASE|OPT_NAME:
            fmt_options |= OPT_NAME;
            break;
        case OPT_BASE|OPT_FORMAT:
            result_format = optarg;
            break;
        case '?':
            eprint(program_name);
            eprint(": ");
            if (is_long_option(argv[this_option_optind])) {
                eprintf("unknown long option, '%s'\n",
                    argv[this_option_optind]);
            }
            else {
                char chrbuf[10];
                eprintf("unknown short option, '%s'\n",
                    vischar_r(chrbuf, sizeof (chrbuf), optopt));
            }
            ++err_count;
            break;
        default:
            eprintf("%s: INTERNAL ERROR: unknown option, ", program_name);
            if (isalpha(optopt)) {
                eprintf("'%c'\n", optopt);
            }
            else {
                eprintf("%d\n", optopt);
            }
            exit(2);
            break;
        }

        if (rv) {
            ++err_count;
        }
    }

    if (err_count) {
        return (err_count);
    }

    verbose = verbose || debug;
    opt_show_argv = opt_show_argv || verbose;
    cmd->verbose = verbose;
    cmd->debug   = debug;

    if (setargv) {
        cmd->cmd_path = argv[optind];
        cmd->cmd_name = sname(cmd->cmd_path);
        cmd->argc = argc - optind;
        cmd->argv = argv + optind;
    }
    return (0);
}

#if 0
void
textbounds_mem(void)
{
    textbox_t textbox;

    test.text = "This is a test\none\ntwo\nthree\n";
    test.siz  = strlen(test.text) + 1;
    test.idx = 0;
    textbox.getchr = (int (*)(void *))textbox_getchr;
    textbox.getchr_arg = (void *)&test;
    textbox.lines = 0;
    textbox.columns = 0;

    text_bounds(&textbox);
    printf("COLUMNS=%zu X LINES=%zu\n", textbox.columns, textbox.lines);
}
#endif

int
textbox_getchr(void *arg)
{
    FILE *srcf = (FILE *)arg;
    return (fgetc(srcf));
}

int
textbounds_filev(size_t filec, char **filev)
{
    fvh_t fv;
    int rv;

    fv.filec = filec;
    fv.filev = filev;
    fv.glnr = 0;
    rv = 0;
    for (fv.fnr = 0; fv.fnr < filec; ++fv.fnr) {
        FILE *srcf;

        fv.fname = fv.filev[fv.fnr];
        if (fv.fname[0] == '-' && fv.fname[1] == '\0') {
            srcf = stdin;
        }
        else {
            srcf = fopen(fv.fname, "r");
            if (srcf == NULL) {
                fprintf(stderr, "fopen('%s', r) failed.\n", fv.fname);
                rv = 2;
                break;
            }
        }

        fv.fh = srcf;
        fv.flnr = 0;

        textbox_t textbox;
        textbox.getchr = (int (*)(void *))textbox_getchr;
        textbox.getchr_arg = (void *)srcf;
        textbox.lines = 0;
        textbox.columns = 0;
        textbox.fmt = result_format;
        textbox.fmt_options = fmt_options;
        text_bounds(&textbox);
        fclose(srcf);
        fshow_textbounds(stdout, fv.fname, &textbox);
    }

    return (rv);
}

void
textbounds_argv(int argc, char **argv)
{
    int rv;

    set_debug_fh("");
    set_eprint_fh();

    // Make it easy to set --debug and --verbose options via the environment,
    // So that it is less likely that options for @command{textboounds}
    // itself are confused with any options to be passed to the the main
    // program.
    //
    if (getenv("TEXTBOUNDS_DEBUG") != NULL) {
        debug = true;
    }
    if (getenv("TEXTBOUNDS_VERBOSE") != NULL) {
        verbose = true;
    }

    rv = textbounds_getopt(cmd, argc, argv, true);

    if (rv != 0) {
        usage();
        exit(1);
    }

    if (opt_show_argv) {
        fshow_str_array(stderr, cmd->argc, cmd->argv);
    }

    if (cmd->ioerr) {
        exit(2);
    }

    textbounds_filev(cmd->argc, cmd->argv);
}
