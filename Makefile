PROGRAM = asteroids
OBJS    = src/main.o src/game.o src/particle.o src/player.o src/bullet.o src/asteroid.o src/collisions.o src/ui.o

CXX      = gcc
CXXFLAGS = -O0 -g -lm -Wall -Wno-varargs -Wno-unused-parameter -Wno-unused-variable
CPPFLAGS = -MMD -Iexternals/include

ifeq ($(OS), Windows_NT)
LDLIBS  = -lraylibdll
LDFLAGS = -Lexternals/libs-Windows

else
LDLIBS  = -lraylib -ldl -lpthread
LDFLAGS = -Lexternals/libs-Unix

endif


DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(PROGRAM) $(OBJS) $(DEPS) && clear