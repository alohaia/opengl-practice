ROOT=.
# PLATFORM=$(shell $(ROOT)/systype.sh)
# include $(ROOT)/Make.defines.$(PLATFORM)
LDLIBS=-lGL -lGLEW -lpthread -ldl -lglfw3 -lX11 -lglut -llua
DEPS=${ROOT}/src/glad.c
CPPC=g++
CC=gcc

PROGS =	restart version restart2 restart3 icon icon2 icon3 restart2-pure test

all: $(PROGS)

restart: restart.cpp $(DEPS)
	$(CPPC) restart.cpp $(DEPS) -o restart $(LDLIBS)

version: version.cpp $(DEPS)
	$(CPPC) version.cpp $(DEPS) -o version $(LDLIBS)

restart2: restart2.cpp $(DEPS)
	$(CPPC) restart2.cpp $(DEPS) -o restart2 $(LDLIBS)

restart3: restart3.cpp $(DEPS)
	$(CPPC) restart3.cpp $(DEPS) -o restart3 $(LDLIBS)

icon: icon.cpp $(DEPS)
	$(CPPC) icon.cpp $(DEPS) -o icon $(LDLIBS)

icon2: icon2.cpp $(DEPS)
	$(CPPC) icon2.cpp $(DEPS) -o icon2 $(LDLIBS)

icon3: icon3.cpp $(DEPS) include/camera.h
	$(CPPC) icon3.cpp $(DEPS) -o icon3 $(LDLIBS)

restart2-pure: restart2-pure.cpp $(DEPS)
	$(CPPC) restart2-pure.cpp $(DEPS) -o restart2-pure $(LDLIBS)

test: test.cpp $(DEPS)
	$(CPPC) test.cpp $(DEPS) -o test $(LDLIBS)


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
