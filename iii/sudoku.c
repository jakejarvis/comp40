/*
 *   sudoku.c
 *
 *   Jake Jarvis (jjarvi01)
 *   Tam Luong   (tluong04)
 *
 *   COMP 40
 *   Homework 2
 *   2/3/15
 *
 */

#include "uarray2.h"
#include <stdlib.h>
#include <stdbool.h>
#include "pnmrdr.h"
#include "assert.h"
#include "mem.h"

void solve_puzzle(FILE *fp);
void add_row(int row, int col, UArray2_T array, void *cl, void *sum);
void add_col(int row, int col, UArray2_T array, void *cl, void *sum);
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

        /* check the type, size, and maximum number of the image file */
        assert(puzzle.type = Pnmrdr_gray);
        assert(puzzle.denominator == 9);
        assert(puzzle.width == 9);
        assert(puzzle.height == 9);

        /* create a 9x9 UArray2 to store the puzzle */
        UArray2_T array = UArray2_new(puzzle.width, puzzle.height, sizeof(int));

        /* iterate through the greymap content and extract the numbers into 
         * a 9x9 UArray2. */
        for(int i = 0; i < (int)(puzzle.width); i++) {
                for(int j = 0; j < (int)(puzzle.height); j++) {
                        int number = Pnmrdr_get(image);

                        int *temp = UArray2_at(array, i, j);
                        *temp = number;
                }
        }

        /* done with the file now that all the numbers are in the array */
        fclose(fp);

        /* this will keep tally of the number as we iterate through the row,
         * column, or box. after each, it will reset to zero. if the puzzle is
         * valid, this will never be greater than 45. */
        int *sum = malloc(sizeof(int));

        UArray2_map_row_major(array, add_row, sum); /* find sum of each row */
        UArray2_map_col_major(array, add_col, sum); /* find sum of each col */
        add_box(array, sum); /* find sum of each box */

        /* clean up memory before exiting */
        Pnmrdr_free(&image);
        UArray2_free(&array);
        FREE(sum);

       // UArray_T 

printf("SOLVED\n");
        exit(0); /* if we've reached the end of this function, the puzzle is
                  * a valid solution. */
}

void add_row(int row, int col, UArray2_T array, void *cl, void *sum)
{
        /* compiler makes us use all the variables */
        (void)cl;

        int *sum_temp = (int *)sum;  /* need to make sum an integer so we can 
                                      * do arithmetic. */

        int *number = UArray2_at(array, row, col);
        *sum_temp += *number;  /* add current number to sum of row */

        if (row == (UArray2_width(array) - 1)) {  /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * row. */
                if (*sum_temp != 45) {
                    //    FREE(sum);
                        UArray2_free(&array);
                        FREE(sum_temp);
                        exit(1);      /* row doesn't add up to 45, puzzle 
                                       * is broken. */
                } else {
                        /* row is valid, reset the sum for the next 
                         * iteration and continue. */
                        *sum_temp = 0;
                        return;
                }
        }
}

void add_col(int row, int col, UArray2_T array, void *cl, void *sum)
{
        /* compiler makes us use all the variables */
        (void)cl;

        int *sum_temp = (int *)sum;  /* need to make sum an integer so we can 
                                      * do arithmetic. */

        int *number = UArray2_at(array, row, col);
        *sum_temp += *number;  /* add current number to sum of column */

        if (col == (UArray2_height(array) - 1)) { /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * column. */
                if (*sum_temp != 45) {
                        exit(1);      /* column doesn't add up to 45, puzzle 
                                       * is broken. */
                } else {
                        /* column is valid, reset the sum for the next 
                         * iteration and continue. */
                        *sum_temp = 0; 
                        return;
                }
        }
}

void add_box(UArray2_T array, int *sum)
{
        for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {    /* this will loop nine times,
                                                 * one for each box. */
                        *sum = 0; /* reset sum before each box */

                        for(int row = 0; row < 3; row++) {
                                for(int col = 0; col < 3; col++) {
                                        /* loop within each box row-by-row */
                                        int *number = UArray2_at(array, 
                                                                (3 * i) + row,
                                                                (3 * j) + col);
                                        /* find the row and column of each 
                                         * number in the context of the entire
                                         * puzzle using i and j as offsets. */

                                        /* add number to sum */
                                        *sum += *number; 
                                }
                        }
                }
        }

        if(*sum != 45) {
                exit(1);   /* box doesn't add up to 45, puzzle is broken. */
        } else {
                *sum = 0;  /* box is valid, reset the sum for the next 
                            * iteration and continue. */
                return;
        }
}