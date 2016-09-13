SHELL = /bin/sh

all: ppmrw.o

ppmrw.o: 
	gcc ppmrw.c -o ppmrw.o

clean :
	rm -f *.o

rebuild: clean ppmrw.o

test6:
	./ppmrw.o 6 triforce.ppm triforce_new.ppm
test3:
	./ppmrw.o 3 P6I.ppm P6I_new.ppm
retest6: clear rebuild test6

clear :
	clear
