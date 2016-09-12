#include <stdio.h>
#include <stdlib.h>

int width;
int height;
int max_value;
int input_type;

typedef struct {
	unsigned char r,g,b;
} Pixel;

char* peek(FILE* fh, char* str){
	int i;
	for (i = 0; i < 10; i += 1){
		str[i] = fgetc(fh);
	}
	for (i = 9; i >= 0; i -= 1){
		ungetc(str[i],fh);
	}
	str[9] = '\0';
	return str;
}

int read_value_from_header(FILE* fh){
	int* i[1];
	fscanf(fh,"%i ",i);
	return *i;
}

void skip_comments(FILE* fh){
	char c;
	peek(fh, &c);
	if (c == '#'){
		while (c = fgetc(fh), c != '\n'){}
	}
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
	
	fprintf(stdout, "W: %d, H: %d, Max: %d\n", width, height,max_value);
	return 0;

}
