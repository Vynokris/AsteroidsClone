OBJS=src/main.o src/game.o src/particle.o src/player.o src/bullet.o src/asteroid.o src/collisions.o src/ui.o
PROGRAM=main

CFLAGS=-O0 -g
CPPFLAGS=-MMD -Iexternals/include
LDLIBS=-lraylib -lm -ldl -lpthread
LDFLAGS=-Lexternals/libs

DEPS=$(OBJS:.o=.d)

all: $(PROGRAM)

%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

-include $(DEPS)

$(PROGRAM): $(OBJS)
	gcc $(CFLAGS) $(CPPFLAGS) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM)