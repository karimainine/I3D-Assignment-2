###################################################################################
# Interactive 3D Graphics and Animation - Assignment 2                            #
# Name: Karim Mohamed Gamal Abulainine                                            #
# Student ID: s3314713                                                            #
###################################################################################

UNAME := $(shell uname)
EXE = i3dAssign2

# Linux
	ifeq ($(UNAME), Linux)
	LDFLAGS = -lGL -lGLU -lglut -std=c99 -lm
endif

# Windows (cygwin)
ifeq ($(UNAME), Windows_NT)
	EXE = $(EXE).exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32 -std=c99
endif

# OS X
ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -std=c99 -m32 -framework QuickTime
endif

all: clean $(EXE) run

$(EXE) : main.c
	gcc -o $@ $< $(LDFLAGS) obj/obj.c boat.c camera.c controls.c keys.c light.c utils.c waves.c texture_common.c texture_quicktime.c seabed.c

clean:
	rm -rf *.o core i3dAssign2 *.errs

run:
	./$(EXE)