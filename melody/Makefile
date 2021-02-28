LLIBS=-lGL -lGLEW -lpthread -ldl -lglfw3 -lX11 -lglut -llua -lfreetype
SRCS=$(VIM_FILEDIR)/src/glad.c
IDIRS=-I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I./include

character: character.cpp Makefile
	g++ -Wall $(SRCS) character.cpp $(IDIRS) $(LLIBS) -o character
