SHELL = /bin/sh

all: ppmrw.o

ppmrw.o: 
	gcc ppmrw.c -o ppmrw.o

clean :
	rm *.o
