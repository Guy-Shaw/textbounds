#include <cscript.h>
#include <stdlib.h>
#include <stdio.h>

extern int textbounds_argv(int, const char * const *av);

const char *program_path;
const char *program_name;

int
main(int argc, const char * const *argv)
{
    int rv;

    program_path = *argv;
    program_name = sname(program_path);
    rv = textbounds_argv(argc, argv);
    dbg_printf("main: rv=%d\n", rv);
    return (rv);
    exit(rv);
}
