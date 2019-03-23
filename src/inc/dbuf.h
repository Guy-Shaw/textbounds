/*
 * Filename: dbuf.h
 * Library: libcscript
 * Brief: Generally handly headers for most libcscript code
 *
 * Copyright (C) 2015-2016 Guy Shaw
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

#ifndef _DBUF_H
#define _DBUF_H

#ifdef  __cplusplus
extern "C" {
#endif

extern char *dbuf_reset(void);
extern char *dbuf_alloc(size_t sz);

#ifdef  __cplusplus
}
#endif

#endif  /* _DBUF_H */
