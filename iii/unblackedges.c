#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include <bit2.h>
#include <stdbool.h>
#include <stack.h>

struct coordinates {
	int x;
	int y;
};

extern Bit2_T pbmread(FILE *fp);
extern void pbmwrite(FILE *fp, Bit2_T bitmap);
extern Bit2_T process_bitmap(Bit2_T bitmap);
extern void print_to_stdout(int i, int j, Bit2_T a, int b, void *p1);
extern bool isBlackedge(Bit2_T orginal, Bit2_T blackedges, struct coordinates c);


int main(int argc, char *argv[])
{
        Bit2_T bitmap;
        if(argc == 1) {    /* no filename, use stdin */
                bitmap = pbmread(stdin);
//		printf("original\n");
//                pbmwrite(stdout, bitmap);
		Bit2_T new_bitmap = process_bitmap(bitmap);
		(void) new_bitmap;
//		printf("processed\n");
		pbmwrite(stdout, new_bitmap);
        } else if (argc == 2) {     /* one filename, use it */
                FILE *fp = fopen(argv[1], "r");
                
                if(fp == NULL) {
                        fprintf(stderr,
                                "ERROR: Could not open file %s for reading.\n",
                                argv[1]);
                } else {
                                // check_validity(fp); /* NOTE BELOW */
                        bitmap = pbmread(fp);
//			printf("original\n");
//                        pbmwrite(stdout, bitmap);
			Bit2_T new_bitmap = process_bitmap(bitmap);
			(void) new_bitmap;
//			printf("processed\n");
			pbmwrite(stdout, new_bitmap);
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
//        printf("hey we are inside the file open!\n");
        Pnmrdr_T image = Pnmrdr_new(fp);
        Pnmrdr_mapdata map = Pnmrdr_data(image);
        Bit2_T temp = Bit2_new(map.width, map.height);
	printf("P1\n");
        printf("%u ",map.width);
        printf("%u\n",map.height);
        for (unsigned int i=0; i<map.height; i++)
                for (unsigned int j=0; j<map.width; j++) {
                        int pixel = Pnmrdr_get(image);
//                      printf("[%i, %i] hey this pixel is: %i\n",i,j,pixel);
                        Bit2_put(temp,j,i,pixel);
                }
//        printf("hey input finishes\n");
        return temp;
}

void print_to_stdout(int i, int j, Bit2_T a, int b, void *p1){
        (void) i;
        (void) j;
        (void) a;
        (void) b;
        (void) p1;
//      	printf("hey we are in print function\n");
        fprintf(stdout, "%u", Bit2_get(a, i, j));
	if (i == Bit2_width(a)-1) {
		fprintf(stdout,"\n");
	}
}

void pbmwrite(FILE *fp, Bit2_T bitmap) {
        (void) fp;
        (void) bitmap;
//      printf("hey we are at a\n");
        Bit2_map_row_major(bitmap, print_to_stdout, NULL);
//      printf("hey we are at b\n");
        
}

extern Bit2_T process_bitmap(Bit2_T bitmap) {
        (void) bitmap;
	int width = Bit2_width(bitmap);
	int height = Bit2_height(bitmap);
	Bit2_T temp = bitmap;
	Bit2_T blackedges = Bit2_new(width, height);
	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			if ( (i==0)||(j==0)||(i==width-1)||(j=height-1) ) {
				if (Bit2_get(bitmap,i,j) == 1) {
					Bit2_put(blackedges,i,j,1);
					Bit2_put(temp,i,j,0);
				} else
					Bit2_put(blackedges,i,j,0);
			}
		}
	}
	for (int i=1; i<width-1; i++) {
		for (int j=1; j<height-1; j++) {
			if (Bit2_get(bitmap,i,j)==1) {
				Stack_T s = Stack_new();
				struct coordinates coord;
				coord.x = i; coord.y = j;
				Stack_push(s, &coord);
				while (Stack_empty(s)==0) {
					struct coordinates *p_coord;
					p_coord = Stack_pop(s);
					int x = p_coord->x;
					int y = p_coord->y;
					if (isBlackedge(bitmap,blackedges,*p_coord)) {
						Bit2_put(temp,x,y,0);
						Bit2_put(blackedges,x,y,1);
						struct coordinates right, left,
						up, down;
						right.x = x+1;
						right.y = y;
						left.x = x-1;
						left.y = y;
						up.x = x;
						up.y = y-1;
						down.x = x;
						down.y = y+1;
						if ( (Bit2_get(blackedges,right.x,right.y)==0) && (Bit2_get(bitmap,right.x,right.y)==1) )
							Stack_push(s, &right);
						if ( (Bit2_get(blackedges,left.x,left.y)==0) && (Bit2_get(bitmap,left.x,left.y)==1) )
							Stack_push(s, &left);
						if ( (Bit2_get(blackedges,up.x,up.y)==0) && (Bit2_get(bitmap,up.x,up.y)==1) )
							Stack_push(s, &up);
						if ( (Bit2_get(blackedges,down.x,down.y)==0) && (Bit2_get(bitmap,down.x,down.y)==1) )
							Stack_push(s, &down);
					}
				}
			}
		}
	}
	
	
	
	
//	printf("blackedges\n");
//	pbmwrite(stdout, blackedges);
	
	
	
	
	
	
	
	
	
        return temp;

}

extern bool isBlackedge(Bit2_T original, Bit2_T blackedges, struct coordinates c) {
	if (Bit2_get(original,c.x,c.y) == 0) return false;
	if (Bit2_get(blackedges,c.x,c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x+1, c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x-1, c.y) == 1) return true;
	if (Bit2_get(blackedges, c.x, c.y+1) == 1) return true;
	if (Bit2_get(blackedges, c.x, c.y-1) == 1) return true;
	return false;
}















