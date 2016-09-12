#include <stdio.h>
#include <stdlib.h>

int width;
int height;
int max_value;
int input_type;

typedef struct {
	unsigned char r,g,b;
} Pixel;

/*
	Used to peek at next char within file. 
	fh: File handle
	c: point to store char
*/
char* peek(FILE* fh, char* c){
	// Pulls first char, stores it, then puts it back.
	*c = fgetc(fh);
	ungetc(*c,fh);
	return c;
}


/*
	Checks if there is comment, if so it skips until a new line. 
	fh: File handle
*/
void skip_comments(FILE* fh){
	char c = 'a';
	// Check next char.
	peek(fh, &c);
	if (c == '#'){
		// Skips until finds new line.
		while (c = fgetc(fh), c != '\n'){}
	}
}

/*
	Reads an int from file.
	fh: File handle
*/
int read_value_from_header(FILE* fh){
	// Checks if next value is comment and skips it.
	skip_comments(fh);
	int* i[1];
	fscanf(fh,"%i ",i);
	// TODO fix compile warning on this.
	return *i;
}


int read_header(FILE* fh){	
	char c;
	c = fgetc(fh);
	// Checks if magic number begins with P.
	if (c != 'P'){
		fprintf(stderr,"Invalid image file, first magic number: %c\n",c);
		return 1;
	}
	// Check if appropiate file formats
	c = fgetc(fh);
	if (c != '3' && c != '6'){
		fprintf(stderr,"Invalid PPM file, PPM file type not supported. %c\n",c);
		return 1;
	}
	// Saves input type as number
	input_type = c - '0';
	
	// Skips down line.
	fgetc(fh);
	// Reads in metadata.
	width = read_value_from_header(fh);
	height = read_value_from_header(fh);
	max_value = read_value_from_header(fh);
	return 0;
}

void read_type_6(FILE* fh, Pixel* buffer){
	int *sub_buffer = malloc(sizeof(int) * width * height);
	fread(sub_buffer,1,width * height, fh);
	int row, col, c;
	for (row = 0; row < height; row += 1){
		for (col = 0; col < width; col += 1){
			c = sub_buffer[row * width + col];
			buffer[row * width + col].r = c >> 24;
			buffer[row * width + col].g = c >> 16 & 0xFF;
			buffer[row * width + col].b = c >> 8 & 0xFF;
		}
	}
}

int main(int argc, char* argv[]){
	// Checks if proper amount of arguments
	if (argc !=  4){
		fprintf(stderr, "Proper Usage: ppmrw output_type input_file output_file\n");
		return 1;
	}
	
	// Opens file and checks if any access problems.
	FILE* fh = fopen(argv[2], "rb");
	if (fh == NULL){
		fprintf(stderr, "File access error\n");
		return 1;
	}
	
	// Checks if had error when reading header.
	int i;
	if (i = read_header(fh), i != 0){
		return i;
	}
	
	// Read out photo metadata
	fprintf(stdout, "W: %d, H: %d, Max: %d\n", width, height,max_value);
	return 0;

}
