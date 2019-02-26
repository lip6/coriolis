CC = gcc
CFLAGS = -O3 -I.

SRC     = dist.c dl.c err.c heap.c mst2.c neighbors.c \
	bookshelf_IO.c memAlloc.c flute.c flute_mst.c
OBJ     = $(SRC:.c=.o)

all: flute-net flute-ckt rand-pts

flute-net: flute-net.c ${OBJ}
	$(CC) $(CFLAGS) -o flute-net flute-net.c ${OBJ} -lm

flute-ckt: flute-ckt.c ${OBJ}
	$(CC) $(CFLAGS) -o flute-ckt flute-ckt.c ${OBJ} -lm

rand-pts: rand-pts.c
	$(CC) $(CFLAGS) -o rand-pts rand-pts.c 

flute.o: flute.c flute.h
	$(CC) $(CFLAGS) -c -o flute.o flute.c

flute_mst.o: flute_mst.c flute.h
	$(CC) $(CFLAGS) -c -o flute_mst.o flute_mst.c

clean:
	rm -f *.o core*
