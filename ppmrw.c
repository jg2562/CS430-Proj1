#include <stdio.h>
#include <stdlib.h>

int width;
int height;
int max_value;
int input_type;

typedef struct {
	unsigned char r,g,b;
} Pixel;


int read_value_from_header(FILE* fh){
	int i;
	fscanf(fh,"=%i",i);
	fprintf(stdout,"fscanf: %i\n",i);
	return i;
}

int read_header(FILE* fh){	
	char c;
	c = fgetc(fh);
	if (c != 'P'){
		fprintf(stderr,"Invalid image file, first magic number: %c\n",c);
		return 1;
	}
	c = fgetc(fh);
	if (c != '3' && c != '6'){
		fprintf(stderr,"Invalid PPM file, PPM file type not supported. %c\n",c);
		return 1;
	}
	input_type = c - '0';
	fgetc(fh);
	width = read_value_from_header(fh);
	height = read_value_from_header(fh);
	max_value = read_value_from_header(fh);
	return 0;
}

int main(int argc, char* argv[]){
	if (argc !=  4){
		fprintf(stderr, "Proper Usage: ppmrw output_type input_file output_file\n");
		return 1;
	}
	FILE* fh = fopen(argv[2], "rb");
	if (fh == NULL){
		fprintf(stderr, "File access error\n");
		return 1;
	}
	int i;
	if (i = read_header(fh), i != 0){
		return i;
	}
	fprintf(stdout, "W: %d, H: %d\n", width, height);
	return 0;

}
