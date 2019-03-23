/*
 * Filename: textbounds.h
 * Brief: Definitions related to textbounds library
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

#ifndef _TEXTBOUNDS_H
#define _TEXTBOUNDS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
    // Import type bool
#include <unistd.h>
    // Import type size_t

typedef unsigned int uint_t;

/*
 * .getchr
 *   Iterator: provides next character of text box
 *
 * .getchr_arg
 *   Argument passed to .getchr().  Optional.  Can be NULL
 *
 * .tws:
 *   Does trailing whitespace count
 *   for purposes of measuring text bounds?
 *   If not, then only the last non-white space character
 *   is used to compute the length of every line.
 *
 * .lines:
 *   Update the number of lines,
 *   AKA the height of the text box area
 *
 * .columns:
 *   Update the number of columns,
 *   AKA the maximum line length,
 *   AKA the width of the text box area
 *
 * Measuring text bounds can be done incrementally.
 * That is, multiple calls to textbounds() can be used to continue
 * measuring text in parts.  The fields, .lines and .columns
 * do not get zeroed out when textboounds() is called.
 *
 * It is the responsibility of the caller to provide initial
 * values for  .lines and .columns.  It is assumed that the most
 * typical case would be to start with { 0, 0 }, then on subsequent
 * calls, choose to continue or to start a new measurement by resetting
 * to { 0, 0 }.
 *
 * .fmt:
 *   If this format string is given (not NULL),
 *   then use it to format the results,
 *   otherwise, format our own way according to the format options.
 *
 * .fmt_options:
 *   Options --name , --lines , --columns
 */

struct textbox {
    // input stream
    int (*getchr)(void *);
    void *getchr_arg;

    // Options governing how measuremnet is done
    bool   tws;         // trailing white space counts

    // Results
    size_t lines;       // Result: how many lines
    size_t columns;     // Result: how many columns

    // Options for formatting results
    char *fmt;
    uint_t fmt_options;
};

typedef struct textbox  textbox_t;

#ifdef  __cplusplus
}
#endif

#endif  /* _TEXTBOUNDS_H */
