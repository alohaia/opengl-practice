ROOT=.
# PLATFORM=$(shell $(ROOT)/systype.sh)
# include $(ROOT)/Make.defines.$(PLATFORM)
LDLIBS=-lGL -lGLEW -lpthread -ldl -lglfw3 -lX11 -lglut `pkg-config --libs lua freetype2`
IINC=-I./include `pkg-config --cflags lua freetype2`
DEPS=${ROOT}/src/glad.c
CPPC=g++
CC=gcc

PROGS =	proj1 proj4-texture restart version restart2 restart3 icon icon2 icon3\
		restart2-pure text test test2 test3 texture_test

all: $(PROGS)

proj1: proj1.cpp $(DEPS)
	$(CPPC) proj1.cpp $(DEPS) -o proj1 $(LDLIBS) $(IINC)

proj4-texture: proj4-texture.cpp $(DEPS)
	$(CPPC) proj4-texture.cpp $(DEPS) -o proj4-texture $(LDLIBS) $(IINC)

restart: restart.cpp $(DEPS)
	$(CPPC) restart.cpp $(DEPS) -o restart $(LDLIBS) $(IINC)

version: version.cpp $(DEPS)
	$(CPPC) version.cpp $(DEPS) -o version $(LDLIBS) $(IINC)

restart2: restart2.cpp $(DEPS)
	$(CPPC) restart2.cpp $(DEPS) -o restart2 $(LDLIBS) $(IINC)

restart3: restart3.cpp $(DEPS)
	$(CPPC) restart3.cpp $(DEPS) -o restart3 $(LDLIBS) $(IINC)

icon: icon.cpp $(DEPS)
	$(CPPC) icon.cpp $(DEPS) -o icon $(LDLIBS) $(IINC)

icon2: icon2.cpp $(DEPS)
	$(CPPC) icon2.cpp $(DEPS) -o icon2 $(LDLIBS) $(IINC)

icon3: icon3.cpp $(DEPS) include/camera.h
	$(CPPC) icon3.cpp $(DEPS) -o icon3 $(LDLIBS) $(IINC)

restart2-pure: restart2-pure.cpp $(DEPS)
	$(CPPC) restart2-pure.cpp $(DEPS) -o restart2-pure $(LDLIBS) $(IINC)

text: text.cpp $(DEPS)
	$(CPPC) text.cpp $(DEPS) -o text $(LDLIBS) $(IINC)

test: test.cpp $(DEPS)
	$(CPPC) test.cpp $(DEPS) -o test $(LDLIBS) $(IINC)

test2: test2.cpp $(DEPS)
	$(CPPC) test2.cpp $(DEPS) -o test2 $(LDLIBS) $(IINC)

test3: test3.cpp $(DEPS)
	$(CPPC) test3.cpp $(DEPS) -o test3 $(LDLIBS) $(IINC)

texture_test: texture_test.cpp $(DEPS)
	$(CPPC) texture_test.cpp $(DEPS) -o texture_test $(LDLIBS) $(IINC)

# myls: myls.c
#     $(CC) $(CFLAGS) myls.c -o myls $(LDLIBS)
#
# # open_max: open_max.cpp
# #     $(CPPC) $(CPPFLAGS) open_max.cpp -o open_max.exe $(LDLIBS) && ./open_max.exe
#
# open_max: open_max.c
#     $(CC) $(CFLAGS) open_max.c -o open_max $(LDLIBS) && ./open_max
#
# alloc_pathname: alloc_pathname.cpp
#     $(CPPC) $(CPPFLAGS) alloc_pathname.cpp -o alloc_pathname $(LDLIBS) && ./alloc_pathname
#
# seek: seek.c Makefile
#     $(CC) $(CFLAGS) seek.c -o seek $(LDLIBS) && ./seek < ./.tasks
#
# test: test.cpp
#     $(CPPC) $(CPPFLAGS) test.cpp -o test $(LDLIBS) && ./test
#
# filehole: filehole.c
#     $(CC) $(CFLAGS) filehole.c -o filehole $(LDLIBS) && ./filehole
#
# setfl:
#     $(CC) $(CFLAGS) setfl.c -o setfl $(LDLIBS) && ./setfl
#
# .PHONY: clean
# clean:
#     rm ${PROGS} *.o a.out

# %: %.cpp
#     echo "$(CPPC) $(CFLAGS) $@.cpp -o $@.exe $(LDLIBS)"

# conf:	conf.c
#
# conf.c:	makeconf.awk sysconf.sym pathconf.sym
#     $(AWK) -f makeconf.awk >conf.c
#
# sysconf.sym:	sysconf-lim.sym
#     grep -v "^#" sysconf-lim.sym >sysconf.sym
#
# pathconf.sym:	pathconf-lim.sym
#     grep -v "^#" pathconf-lim.sym >pathconf.sym
#
# options:	options.c
#
# options.c:	makeopt.awk sysopt.sym pathopt.sym
#     $(AWK) -f makeopt.awk >options.c
#
# sysopt.sym:	sysconf-opt.sym
#     grep -v "^#" sysconf-opt.sym >sysopt.sym
#
# pathopt.sym:	pathconf-opt.sym
#     grep -v "^#" pathconf-opt.sym >pathopt.sym
#
clean:
	rm -f $(PROGS) a.out *.o

# include $(ROOT)/Make.libapue.inc
