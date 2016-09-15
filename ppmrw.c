#include <stdio.h>
#include <stdlib.h>

int input_type;
int output_type;

typedef struct {
	unsigned char r,g,b;
} Pixel;

typedef struct{
	int width, height, max_value;
	Pixel* buffer;
} Image;

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
int read_value_from_header(FILE* fh, int i){
	// Checks if next value is comment and skips it.
	skip_comments(fh);
	fscanf(fh,"%i ",&i);
	return i;
}

/*
	Reads in a type 3 PPM file to buffer.
	fh: file handle
	buffer: buffer to store file data. 
*/
void read_type_3(FILE* fh, Image* img){
	int row, col;
	int *a = malloc(sizeof(int));
	for (row = 0; row < img->height; row += 1){
		for (col = 0; col < img->width; col += 1){
			// Changes data from binary to Pixel structure format
			Pixel pix = img->buffer[row * img->width + col];
			fscanf(fh, "%i ", a);
			pix.r = *a;
			fscanf(fh, "%i ", a);
			pix.g = *a;
			fscanf(fh, "%i ", a);
			pix.b = *a;
			img->buffer[row * img->width + col] = pix;
		}
	}
}

/*
	Reads in a type 6 PPM file to buffer.
	fh: file handle
	buffer: buffer to store file data. 
*/
void read_type_6(FILE* fh, Image *img){
	// Creates a sub buffer to read in all file data
	int *sub_buffer = malloc(sizeof(int) * img->width * img->height);
	// Reading in file data
	fread(sub_buffer,sizeof(Pixel),img->width * img->height, fh);
	int row, col, bin;
	for (row = 0; row < img->height; row += 1){
		for (col = 0; col < img->width; col += 1){
			// Changes data from binary to Pixel structure format
			bin = sub_buffer[row * img->width + col];
			Pixel pix;
			// pix.a = 0xFF;
			pix.b = bin >> 16 & 0xFF;
			pix.g = bin >> 8 & 0xFF;
			pix.r = bin & 0xFF;
			img->buffer[row * img->width + col] = pix;
			
		}
	}
	
	// Frees buffer
	free(sub_buffer);
}

/*
	Reads in an image file to the Image structure
	fh: file handle
	img: Image structure to store image data
*/
int read_file(FILE* fh, Image* img){	
	char c;
	c = fgetc(fh);
	// Checks if magic number begins with P.
	if (c != 'P'){
		fprintf(stderr,"Invalid image file, first magic number: %c\n",c);
		return 1;
	}
	// Check if appropiate file formats
	c = fgetc(fh);
	input_type = atoi(&c);
	
	// Skips down line.
	fgetc(fh);
	
	// Reads in metadata.
	int i;
	img->width = read_value_from_header(fh, i);
	img->height = read_value_from_header(fh, i);
	img->max_value = read_value_from_header(fh, i);
	// Create buffer to store photo
	img->buffer = malloc(sizeof(Pixel) * img->width * img->height);
	switch(input_type){
		case 3:
			read_type_3(fh, img);
			return 0;
		case 6:
			read_type_6(fh, img);
			return 0;
		default:
			fprintf(stderr,"Invalid PPM file, PPM file type not supported. %c\n",c);
			return 1;
	}
}

/*
	Writes a P3 image from an Image structure to an file
	fh: file handle
	img: Image structure storing image data
*/
void write_type_3(FILE* fh, Image* img){
	int row, col, a;
	for (row = 0; row < img->height; row += 1){
		for (col = 0; col < img->width; col += 1){
			// Changes data from binary to Pixel structure format
			Pixel pix = img->buffer[row * img->width + col];
			fprintf(fh, "%i\n", pix.r);
			fprintf(fh, "%i\n", pix.g);
			fprintf(fh, "%i\n", pix.b);
		}
	}
	
}

/*
	Writes a P6 image from an Image structure to an file
	fh: file handle
	img: Image structure storing image data
*/
void write_type_6(FILE* fh, Image* img){
	// Creates a sub_buffer to write all data to before file
	int *sub_buffer = malloc(sizeof(Pixel) * img->width * img->height);
	// Reading in file data
	int row, col;
	Pixel pix;
	for (row = 0; row < img->height; row += 1){
		for (col = 0; col < img->width; col += 1){
			pix = img->buffer[row * img->width + col];
			// Or bytes together to transform Pixel structure to binary
			// sub_buffer[row * img->width + col] = pix.a << 24 | pix.b << 16 | pix.g << 8 | pix.r;
			sub_buffer[row * img->width + col] = pix.b << 16 | pix.g << 8 | pix.r;
		}
	}
	// Writes sub_buffer to file
	fwrite(sub_buffer,sizeof(Pixel),img->width * img->height, fh);
	
	// Frees buffer
	free(sub_buffer);
}


/* 
	Writes image structure into output file
	fh: output file handle
	img: image structure to write
	output_type: the type of file to export as
*/
int write_file(FILE* fh, Image* img, int output_type){
	// writes out header to file stream
	fprintf(fh, "P%i\n%i %i\n%i\n", output_type, img->width, img->height, img->max_value);
	switch(output_type){
		case 3:
			write_type_3(fh, img);
			return 0;
		case 6:
			write_type_6(fh, img);
			return 0;
		default:
			fprintf(stderr,"Invalid PPM output type.\n");
			return 1;
		
	}
}


int main(int argc, char* argv[]){
	// Checks if proper amount of arguments
	if (argc !=  4){
		fprintf(stderr, "Proper Usage: ppmrw output_type input_file output_file\n");
		return 1;
	}
	
	if (output_type = atoi(argv[1]), output_type != 3 && output_type != 6){
		fprintf(stderr, "Invalid file type for output file.\n");
		return 1;
	}
	
	// Opens file and checks if any access problems.
	FILE* in = fopen(argv[2], "rb");
	if (in == NULL){
		fprintf(stderr, "File access error\n");
		return 1;
	}
	FILE* out = fopen(argv[3], "wb");
	if (out == NULL){
		fprintf(stderr, "File write error\n");
	}
	
	
	Image img;
	// Checks if had error when reading header.
	int i;
	if (i = read_file(in, &img), i != 0){
		return i;
	}
	
	
	// Writes photo to file
	if (i = write_file(out, &img, output_type), i != 0){
		return i;
	}
	
	// Clean up
	free(img.buffer);
	return 0;
}
