
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
PROGRAMS := $(patsubst %.c,%,$(SOURCES))
DISTBIN := /usr/local/bin

CC := gcc
CPPFLAGS := -I../inc
CFLAGS := -Wall -Wextra -g

LIBCSCRIPT := ../libcscript/libcscript.a

.PHONY: all cscope clean install show-targets

all: $(PROGRAMS)

$(PROGRAMS): $(OBJECTS) ../libtextbounds/libtextbounds.a $(LIBCSCRIPT) -lexplain

../libtextbounds/libtextbounds.a:
	cd ../libtextbounds && make libtextbounds.a

../libcscript/libcscript.a:
	cd ../libcscript && make libcscript.a

valgrind:
	valgrind --leak-check=full ./ush --fork ls-script-text

cscope:
	cscope-init

clean:
	cd ../libtextbounds && make clean
	rm -f $(PROGRAMS) *.o

install: all
	cp $(PROGRAMS) $(DISTBIN)
	cd $(DISTBIN) && chmod 755 $(PROGRAMS)

show-targets:
	@show-makefile-targets

show-%:
	@echo $*=$($*)

#END
