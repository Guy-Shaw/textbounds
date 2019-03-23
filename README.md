# textbounds -- height and width of text

`textbounds` measures the height and width of a piece of text.
Height is number of lines and width is the length of the longest line,
also know as columns.

`libtextbounds` is a library that implements the heart of the
logic.  The function `textbounds` can be fed a stream of text.
It does not know where the text comes from, originally.
The test can come from a piece of text in memory or from a file.

In addition to the library, there is a main driver program
that take filenames and various options.  But the library
can be used without the main program, and a small subset of
the library can be used.

## Main program, standard options

The command, `textbounds` takes an optional list of files.
It recognizes options that specify how to show the filename,
lines and columns.

--help

Show a help message on stderr, explaining these options.

--version

Show version information, then exit.

--debug

Pretty-print values of interest only for debugging.

--verbose

Show some feedback while running.

## Main program, textbounds display options

--format=_formatspecifier_

Where format specifier is something sort of like a printf
format, except that the only format placeholders are
 { %f , %l , %c , and %% }.

1. %f gets replaced with the filename
2. %l gets replaced with the number of lines
3. %c gets replaced with the number of columns

If no `--format` is specified, then a builtin format
is created from any combination of the options:
 `--name` , `--lines` , `--columns`.

--name

Show the filename

--lines

Show the number of lines

--columns

Show the number of columns

If none of the above formatting options is specified,
then the default format is "%cx%l".


## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

##

-- Guy Shaw

   gshaw@acm.org

