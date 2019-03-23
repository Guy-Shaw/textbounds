/*
 * Filename: textbounds.c
 * Library: libtextbounds
 * Brief: Compute lines and columns of a stream of text.
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

#include <textbounds.h>
#include <stdio.h>
    // Import constant EOF
#include <unistd.h>
    // Import type size_t

void
text_bounds(textbox_t *ctxp) {
    size_t lnr = 0;
    size_t col = 0;       /* last column - even if just whitespace */
    size_t inkcol = 0;    /* last non-whitespace column */
    size_t maxcol = 0;
    int c;

    do {
        c = (*ctxp->getchr)(ctxp->getchr_arg);
        switch(c) {
            case EOF:
            case '\n':
                if (c == '\n' || col > 0) {
                    ++lnr;
                }
                if (ctxp->tws) {
                    if (col > maxcol) {
                        maxcol = col;
                    }
                }
                else {
                    if (inkcol > maxcol) {
                        maxcol = inkcol;
                    }
                }
                col = inkcol = 0;
                break;
            case '\t':
                col = (col + 8) & ~7;
                break;
            case ' ':
                ++col;
                break;
            default:
                ++col;
                inkcol = col;
        }
    } while (c != EOF);

    ctxp->lines   = lnr;
    ctxp->columns = maxcol;
}
