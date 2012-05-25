###################################################################################
# Interactive 3D Graphics and Animation - Assignment 2                            #
# Name: Karim Mohamed Gamal Abulainine                                            #
# Student ID: s3314713                                                            #
###################################################################################

UNAME := $(shell uname)
EXE = i3dAssign2
TEXTURE_FILE = texture_gdk.c

# Linux
	ifeq ($(UNAME), Linux)
	LDFLAGS = -lGL -lGLU -lglut -std=c99 -lm -lz
endif

# Windows (cygwin)
ifeq ($(UNAME), Windows_NT)
	EXE = $(EXE).exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32 -std=c99
endif

# OS X
ifeq ($(UNAME), Darwin)
	TEXTURE_FILE = texture_quicktime.c
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -std=c99 -m32 -framework QuickTime -lz
endif

all: clean $(EXE) run

$(EXE) : main.c
	gcc -o $@ $< $(LDFLAGS) $(TEXTURE_FILE) obj/obj.c boat.c camera.c controls.c keys.c light.c utils.c skybox.c waves.c texture_common.c seabed.c png_loader.c cannon_ball.c

clean:
	rm -rf *.o core i3dAssign2 *.errs

run:
	./$(EXE)