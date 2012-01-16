ifeq ($(shell uname),Darwin)
	LDFLAGS:=-framework GLUT -framework OpenGL
else
	LDFLAGS:=-lglut -lGLU
endif

all: hw3b

hw3b: hw3b.o glm.o
	g++ hw3b.o glm.o $(LDFLAGS) -o hw3b 
