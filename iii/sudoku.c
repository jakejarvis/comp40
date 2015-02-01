
#include "uarray2.h"
#include <stdlib.h>
#include "pnmrdr.h"
#include "assert.h"

void solve_puzzle(FILE *fp);
void add_row(int i, int j, UArray2_T array, void *cl, void *sum);
void add_col(int i, int j, UArray2_T array, void *cl, void *sum);
void add_box(UArray2_T array, int *sum);

int main(int argc, char *argv[])
{
        if(argc == 1) {    /* no filename, use stdin */
                solve_puzzle(stdin);
        } else if (argc == 2) {     /* one filename, use it */
                FILE *fp = fopen(argv[1], "r");

                if(fp == NULL) {
                        fprintf(stderr,
                                "ERROR: Could not open file %s for reading.\n",
                                argv[1]);
                } else {
                        // check_validity(fp); /* NOTE BELOW */
                        solve_puzzle(fp);
                        fclose(fp);
                }
        } else {    /* more than one filename, abort */
                fprintf(stderr,
                        "ERROR: Please only give one filename at a time.\n");
        }

        return 0;
}

void solve_puzzle(FILE *fp)
{
        Pnmrdr_T image = Pnmrdr_new(fp);
        Pnmrdr_mapdata puzzle = Pnmrdr_data(image);

        assert(puzzle.type = Pnmrdr_gray);
        assert(puzzle.denominator == 9);
        assert(puzzle.width == 9);
        assert(puzzle.height == 9);

        UArray2_T array = UArray2_new(puzzle.width, puzzle.height, sizeof(int));

        for(int i = 0; i < (int)(puzzle.width); i++) {
                for(int j = 0; j < (int)(puzzle.height); j++) {
                        int number = Pnmrdr_get(image);

                        int *temp = UArray2_at(array, i, j);
                        *temp = number;
                }
        }

        int *sum = 0;

        UArray2_map_row_major(array, add_row, sum);
        UArray2_map_col_major(array, add_col, sum);
        add_box(array, sum);

        exit(0);
}

void add_row(int i, int j, UArray2_T array, void *cl, void *sum)
{
        (void)cl;

        int *number = UArray2_at(array, i, j);

        int *sum_temp = (int *)sum;
        *sum_temp += *number;

        if(i == (UArray2_width(array) - 1)) {
                if(*sum_temp != 45) {
                        exit(1);
                } else {
                        *sum_temp = 0;
                        return;
                }
        }
}

void add_col(int i, int j, UArray2_T array, void *cl, void *sum)
{
        (void)cl;

        int *number = UArray2_at(array, i, j);

        int *sum_temp = (int *)sum;
        *sum_temp += *number;

        if(j == (UArray2_height(array) - 1)) {
                if(*sum_temp != 45) {
                        exit(1);
                } else {
                        *sum_temp = 0;
                        return;
                }
        }
}

void add_box(UArray2_T array, int *sum)
{
        for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                        for(int row = 0; row < 3; row++) {
                                for(int col = 0; col < 3; col++) {
                                        int *number = UArray2_at(array, (3 * i) + row, (3 * j) + col);
                                        *sum += *number;
                                }
                        }
                }
        }

        if(*sum != 45) {
                exit(1);
        } else {
                *sum = 0;
                return;
        }
}