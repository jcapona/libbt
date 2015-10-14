# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Authors: Felipe Ortiz
# Version: 0.9

CXX=g++ -std=c++11
INCLUDES=-Iinclude
CXXOPTS=-Wall -g -O2
CXXFLAGS=$(CXXOPTS) $(INCLUDES)
LDFLAGS=
LDLIBS=
# Where .a file must be created
LIB=lib/libbt.a

default: all
# Object files
OBJS=src/bt.o

all: lib $(LIB)

lib:
	mkdir lib

$(LIB): $(LIB)($(OBJS))

clean:
	rm -f lib/*.a src/*.o

%.d: %.cc
	$(CXX) -MM -MP -MF $@ -MT "$(@:.d=.o) $@" $(INCLUDES) $<

ifneq "$(MAKECMDGOALS)" "clean"
 -include $(OBJS:.o=.d)
endif
