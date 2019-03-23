
#include <cscript.h>
#include <textbounds.h>

#include <stdio.h>
    // Import constant EOF
    // Import printf()
#include <stdlib.h>
    // Import exit()
#include <string.h>
    // Import strlen()
#include <unistd.h>
    // Import type size_t

extern int text_bounds(textbox_t *);

const char *program_path;
const char *program_name;

struct text_iterator {
    char *text;
    size_t siz;
    size_t idx;
};

typedef struct text_iterator text_iterator_t;

static text_iterator_t test;

static int
textbox_getchr(text_iterator_t *it)
{
    int chr;

    if (it->idx >= it->siz) {
        chr = EOF;
    }
    else {
        chr = it->text[it->idx];
        ++it->idx;
    }
    return (chr);
}

int
main(int argc, const char * const *argv)
{
    textbox_t textbox;
    int rv;

    (void)argc;

    program_path = *argv;
    program_name = sname(program_path);
    rv = 0;

    test.text = "This is a test\none\ntwo\nthree\n";
    test.siz  = strlen(test.text) + 1;
    test.idx = 0;
    textbox.getchr = (int (*)(void *))textbox_getchr;
    textbox.getchr_arg = (void *)&test;
    textbox.lines = 0;
    textbox.columns = 0;

    text_bounds(&textbox);
    printf("COLUMNS=%zu X LINES=%zu\n", textbox.columns, textbox.lines);
    // dbg_printf("main: rv=%d\n", rv);
    return (rv);
    exit(rv);
}
