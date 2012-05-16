# Note: Makefile only compatible with gnu-make

# Basic flags
CC := gcc
CFLAGS := -Wall -std=c99
LDFLAGS := -lGL -lGLU -lglut -lm
OPTFLAGS := -O2
DBGFLAGS := -g -DDEBUG
RM := rm -f

# Executable
EXE := i3d_ass1

# Files
OBJECTS := boat.o camera.o controls.o keys.o light.o main.o utils.o waves.o obj/obj.o
HEADERS := boat.h camera.h gl.h keys.h light.h utils.h waves.h obj/obj.h obj/uthash.h
SOURCES := $(OBJECTS:%.o=%.c)

# For compiling different versions
dbg := n
opt := n

ifeq ($(dbg), y)
	CFLAGS += $(DBGFLAGS)
endif
ifeq ($(opt), y)
	CFLAGS += $(OPTFLAGS)
endif

# Compile command
all : depend $(OBJECTS)
	$(CC) -o $(EXE) $(LDFLAGS) $(CFLAGS) $(OBJECTS)

# Rule for compiling object files
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# This could also be done with -include
depend:
	@sed -itmp '1,/^# DO NOT DELETE/! d' Makefile
	@rm Makefiletmp
	$(CC) -MM $(CFLAGS) $(SOURCES) >> Makefile

# Removing object files
.PHONY : clean
clean:
	$(RM) $(OBJECTS)

# Auto generated dependency information created by make depend:
# DO NOT DELETE!
boat.o: boat.c boat.h utils.h controls.h waves.h gl.h obj/obj.h
camera.o: camera.c camera.h utils.h gl.h
controls.o: controls.c controls.h utils.h
keys.o: keys.c keys.h utils.h
light.o: light.c light.h utils.h gl.h
main.o: main.c gl.h camera.h utils.h waves.h light.h boat.h keys.h \
 controls.h
utils.o: utils.c utils.h
waves.o: waves.c waves.h utils.h gl.h
obj.o: obj/obj.c obj/obj.h obj/uthash.h
