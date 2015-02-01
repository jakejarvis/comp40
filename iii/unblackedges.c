#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <bit2.h>

Bit2_T pbmread(FILE *fp);
void pbmwrite(FILE *fp, Bit2_T bitmap);
extern Bit2_T process_bitmap(Bit2_T bitmap);
void print_to_file(int i, int j, Bit2_T a, int b, void *p1);


int main(int argc, char *argv[])
{
	Bit2_T bitmap;
	if(argc == 1) {    /* no filename, use stdin */
		bitmap = pbmread(stdin);
//		Bit2_T new_bitmap = process_bitmap(bitmap);
		pbmwrite(stdout, bitmap);
	} else if (argc == 2) {     /* one filename, use it */
		FILE *fp = fopen(argv[1], "r");
		
		if(fp == NULL) {
			fprintf(stderr,
				"ERROR: Could not open file %s for reading.\n",
				argv[1]);
		} else {
				// check_validity(fp); /* NOTE BELOW */
			bitmap = pbmread(fp);
//			Bit2_T new_bitmap = process_bitmap(bitmap);
			pbmwrite(stdout, bitmap);
			fclose(fp);
		}
	} else {    /* more than one filename, abort */
		fprintf(stderr,
			"ERROR: Please only give one filename at a time.\n");
	}
	
	
	
	return 0;
}

extern Bit2_T pbmread(FILE *fp)
{
	printf("hey we are inside the file open!\n");
	Pnmrdr_T image = Pnmrdr_new(fp);
	Pnmrdr_mapdata map = Pnmrdr_data(image);
	Bit2_T temp = Bit2_new(map.width, map.height);
	printf("hey the width is: %u\n",map.width);
	printf("hey the height is: %u\n",map.height);
	for (unsigned int i=0; i<map.width; i++)
		for (unsigned int j=0; j<map.height; j++) {
			int pixel = Pnmrdr_get(image);
//			printf("[%i, %i] hey this pixel is: %i\n",i,j,pixel);
			Bit2_put(temp,i,j,pixel);
		}
	printf("hey input finishes\n");
	return temp;
}

void print_to_file(int i, int j, Bit2_T a, int b, void *p1){
	(void) i;
	(void) j;
	(void) a;
	(void) b;
	(void) p1;
//	printf("hey we are in print function\n");
	fprintf(stdout, "%u", Bit2_get(a, i, j));
}

void pbmwrite(FILE *fp, Bit2_T bitmap) {
	(void) fp;
	(void) bitmap;
//	printf("hey we are at a\n");
	Bit2_map_col_major(bitmap, print_to_file, NULL);
//	printf("hey we are at b\n");
	
}

extern Bit2_T process_bitmap(Bit2_T bitmap) {
	(void) bitmap;
	Bit2_T temp = Bit2_new(2,2);
	return temp;

}











