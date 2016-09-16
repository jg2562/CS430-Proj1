SHELL = /bin/sh

build: 
	gcc ppmrw.c -o ppmrw.o

clean:
	rm -f *.o
	rm -f new_*.ppm

clear :
	clear

rebuild: clean build

p3to3:
	./ppmrw.o 3 P3I.ppm new_P3I_3.ppm
p3to6:
	./ppmrw.o 6 P3I.ppm new_P6I_3.ppm
p6to3:
	./ppmrw.o 3 P6I.ppm new_P3I_6.ppm
p6to6:
	./ppmrw.o 6 P6I.ppm new_P6I_6.ppm
	
fullTest: rebuild clear p3to3 p3to6 p6to3 p6to6