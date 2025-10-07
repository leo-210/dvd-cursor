CFLAGS = -Wall
INC = -I/usr/include

all: dvd-cursor

dvd-cursor: main.o
	gcc $(CFLAGS) main.o -o dvd-cursor

main.o: src/main.c src/config.h dirs
	gcc $(CFLAGS) -c ./src/main.c -o main.o

clean:
	rm -rf *.o
	rm -rf dvd-cursor

.PHONY: dirs clean all
