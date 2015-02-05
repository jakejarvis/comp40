#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <bit2.h>
#include <stdbool.h>
#include <stack.h>
#include <mem.h>

struct coordinates {
	int x;
	int y;
};

extern Bit2_T pbmread(FILE *fp);
extern void pbmwrite(FILE *fp, Bit2_T bitmap);
extern void process_bitmap(Bit2_T bitmap);
extern void print_to_stdout(int i, int j, Bit2_T a, int b, void *p1);
extern bool isBlackedge(Bit2_T orginal, Bit2_T blackedges, struct coordinates c);


int main(int argc, char *argv[])
{
        Bit2_T bitmap;
        if(argc == 1) {    /* no filename, use stdin */
                bitmap = pbmread(stdin);
		process_bitmap(bitmap);
		pbmwrite(stdout, bitmap);
		Bit2_free(&bitmap);
        } else if (argc == 2) {     /* one filename, use it */
                FILE *fp = fopen(argv[1], "r");
                
                if(fp == NULL) {
                        fprintf(stderr,
                                "ERROR: Could not open file %s for reading.\n",
                                argv[1]);
                } else {
                        bitmap = pbmread(fp);
			process_bitmap(bitmap);
			pbmwrite(stdout, bitmap);
			Bit2_free(&bitmap);
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
        Pnmrdr_T image = Pnmrdr_new(fp);
        Pnmrdr_mapdata map = Pnmrdr_data(image);
        Bit2_T temp = Bit2_new(map.width, map.height);

	printf("P1\n"); /* print bitmap type - always P1 */
        printf("%u ",map.width);    /* print width + whitespace + height */
        printf("%u\n",map.height);
        for (unsigned int i=0; i<map.height; i++) {
                for (unsigned int j=0; j<map.width; j++) {
                        int pixel = Pnmrdr_get(image);
                        Bit2_put(temp,j,i,pixel);
                }
        }
	Pnmrdr_free(&image);
        return temp;
}

void print_to_stdout(int i, int j, Bit2_T a, int b, void *p1){
        (void)b;
        (void)p1;

        fprintf(stdout, "%u", Bit2_get(a, i, j));
	if (i == Bit2_width(a) - 1) {
		fprintf(stdout,"\n");
	}
}

void pbmwrite(FILE *fp, Bit2_T bitmap) {
        (void)fp;

        Bit2_map_row_major(bitmap, print_to_stdout, NULL);        
}

extern void process_bitmap(Bit2_T bitmap) {
        (void) bitmap;
	int width = Bit2_width(bitmap);
	int height = Bit2_height(bitmap);

	Bit2_T blackedges = Bit2_new(width, height);
	for (int i = 0; i<width; i++) {
		for (int j = 0; j<height; j++) {
			if ( (i == 0) || (j == 0) || (i == width - 1) 
                                                  || (j == height - 1) ) {
				if (Bit2_get(bitmap,i,j) == 1) {
					Bit2_put(blackedges, i, j, 1);
					Bit2_put(bitmap, i, j, 0);
				} else
					Bit2_put(blackedges, i, j, 0);
			}
		}
	}
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			if (Bit2_get(bitmap, i, j) == 1) {
				Stack_T s = Stack_new();
				struct coordinates coord;
				coord.x = i; coord.y = j;
				Stack_push(s, &coord);
				while (Stack_empty(s)==0) {
					struct coordinates *p_coord;
					p_coord = Stack_pop(s);
					
					int x = p_coord->x;
					int y = p_coord->y;
					
					struct coordinates coord;
					coord.x = x;
					coord.y = y;
					
					if (isBlackedge(bitmap,
                                                        blackedges,
                                                        coord)) {
						struct coordinates *right,
                                                                   *left,
                   						   *up, 
                                                                   *down;

						right = calloc(1, 
                                                   sizeof(struct coordinates));
						left = calloc(1, 
                                                   sizeof(struct coordinates));
						up = calloc(1, 
                                                   sizeof(struct coordinates));
						down = calloc(1, 
                                                   sizeof(struct coordinates));
						right->x = x+1;
						right->y = y;
						left->x = x-1;
						left->y = y;
						up->x = x;
						up->y = y-1;
						down->x = x;
						down->y = y+1;
						Bit2_put(bitmap, x, y, 0);
						Bit2_put(blackedges, x, y, 1);
						if ((Bit2_get(blackedges, 
                                                              right->x,
                                                              right->y) == 0) 
                                                 && (Bit2_get(bitmap, 
                                                              right->x,
                                                              right->y) == 1)) {
							Stack_push(s, right);
						} else {
							FREE(right);
						}
						if ((Bit2_get(blackedges,
                                                              left->x,
                                                              left->y) == 0) 
                                                 && (Bit2_get(bitmap,
                                                              left->x,
                                                              left->y) == 1)) {
							Stack_push(s, left);
						} else {
							FREE(left);
						}
						if ((Bit2_get(blackedges,
                                                              up->x,
                                                              up->y) == 0) 
                                                 && (Bit2_get(bitmap,
                                                              up->x,
                                                              up->y) == 1)) {
							Stack_push(s, up);
						} else {
							FREE(up);
						}
						if ((Bit2_get(blackedges,
                                                              down->x,
                                                              down->y) == 0)
                                                 && (Bit2_get(bitmap,
                                                              down->x,
                                                              down->y) == 1)) {
							Stack_push(s, down);
						} else {
							FREE(down);
						}
					}
				}
				Stack_free(&s);
			}
		}
	}
	
	Bit2_free(&blackedges);
}

extern bool isBlackedge(Bit2_T original, 
                        Bit2_T blackedges, 
                        struct coordinates c) 
{
	if (Bit2_get(original, c.x, c.y) == 0) return false;
	if (Bit2_get(blackedges, c.x, c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x + 1, c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x - 1, c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x, c.y + 1) == 1) return true;
	if (Bit2_get(blackedges, c.x, c.y - 1) == 1) return true;
	return false;
}
