# Filename: src/Makefile
# Project: textbounds
# Brief: Top-lovel Makefile for textbounds -- build libraries and commands
#
# Copyright (C) 2019 Guy Shaw
# Written by Guy Shaw <gshaw@acm.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

LIBRARIES := libtextbounds/libtextbounds.a   libcscript/libcscript.a

.PHONY: all .FORCE clean show-targets


all: $(LIBRARIES) test/test-textbounds cmd/textbounds

libtextbounds/libtextbounds.a:
	cd libtextbounds && make

libcscript/libcscript.a:
	cd libcscript && make

cmd/textbounds: $(LIBRARIES)
	cd cmd && make

test/test-textbounds: $(LIBRARIES)
	cd test && make

clean:
	cd libtextbounds && make clean
	cd libcscript && make clean
	cd test && make clean
	cd cmd && make clean

.FORCE:

show-targets:
	@show-makefile-targets

show-%:
	@echo $*=$($*)
