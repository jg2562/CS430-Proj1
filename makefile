SHELL = /bin/sh

all: ppmrw.o

ppmrw.o: 
	gcc ppmrw.c -o ppmrw.o

clean :
	rm -f *.o
	rm -f new_*.ppm

rebuild: clean ppmrw.o

test6:
	./ppmrw.o 6 P6I.ppm new_P6I.ppm
test3:
	./ppmrw.o 3 P3I.ppm new_P3I.ppm
retest6: clear rebuild test6

clear :
	clear
