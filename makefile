CFLAGS = -Wall
INC = -I/usr/include

all: bin/dvdcursor

bin/dvdcursor: build/main.o
	gcc $(CFLAGS) build/main.o -o bin/dvdcursor

build/main.o: src/main.c
	gcc $(CFLAGS) -c ./src/main.c -o build/main.o

dirs:
	@mkdir build
	@mkdir bin

clean:
	rm -rf build/*.o
	rm -rf bin/dvdcursor

.PHONY: dirs clean all
