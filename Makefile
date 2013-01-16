CC      = gcc
CFLAGS  = -std=c99 -Wall -Werror -O0
LDFLAGS =
EXEC    = control-tower pilot atis
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

all: $(EXEC)

control-tower: src/control-tower.o src/shared.o
	$(CC) -o $@ $^ $(LDFLAGS)

pilot: src/pilot.o src/shared.o
	$(CC) -o $@ $^ $(LDFLAGS)

atis: src/atis.o src/shared.o
	$(CC) -o $@ $^ $(LDFLAGS)

control-tower.o: src/shared.h

pilot.o: src/shared.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm src/*.o

mrproper: clean
	rm $(EXEC)
	rm fifo-in
