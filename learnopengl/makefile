CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -I../../../third-party/libdrop/include
LDFLAGS=-g -L../../../third-party/libdrop/lib
LDLIBS=-lGLEW -lglfw3 -lSOIL
FRAMEWORKS= -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRCS=main.cpp shader.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: learnopengl.camera clean

learnopengl.camera: $(OBJS)
	$(CXX) $(LDFLAGS) -o learnopengl.camera $(OBJS) $(LDLIBS) $(FRAMEWORKS)

main.o: main.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp

shader.o: shader.cpp shader.h
	$(CXX) $(CPPFLAGS) -c shader.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) tool
