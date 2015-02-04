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
#include "uarray.h"

void solve_puzzle(FILE *fp);

void add_row(int row, int col, UArray2_T array, void *cl, void *sum);
void add_col(int row, int col, UArray2_T array, void *cl, void *sum);
void add_box(UArray2_T array, int *sum);

void check_row_duplicates(int row, int col, UArray2_T array, void *cl, 
                                                             void *tracker);
void check_col_duplicates(int row, int col, UArray2_T array, void *cl, 
                                                             void *tracker);
void check_box_duplicates(UArray2_T array, void *tracker);

int main(int argc, char *argv[])
{
        if (argc == 1) {    /* no filename, use stdin */
                solve_puzzle(stdin);
        } else if (argc == 2) {     /* one filename, use it */
                FILE *fp = fopen(argv[1], "r");

                if (fp == NULL) {
                        fprintf(stderr,
                                "ERROR: Could not open file %s for reading.\n",
                                argv[1]);
                } else {
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
        for (int i = 0; i < (int)(puzzle.width); i++) {
                for (int j = 0; j < (int)(puzzle.height); j++) {
                        int num = Pnmrdr_get(image);

                        int *temp = UArray2_at(array, i, j);
                        *temp = num;
                }
        }

        /* done with the file now that all the numbers are in the array */
        fclose(fp);

        /* this will keep tally of the number as we iterate through the row,
         * column, or box. after each, it will reset to zero. if the puzzle is
         * valid, this will never be greater than 45. */
        int *sum = calloc(1, sizeof(int));

        /* checking if each row, col, and box totals to 45 is a quick way of 
         * eliminating incorrect puzzles -- however it doesn't catch all of 
         * them. after this, we still need to check for duplicate numbers. */
        UArray2_map_row_major(array, add_row, sum); /* find sum of each row */
        UArray2_map_col_major(array, add_col, sum); /* find sum of each col */
        add_box(array, sum); /* find sum of each box */

        /* if the program hasn't exited by this point, this means that each 
         * row, column, and box equals 45. however, this doesn't mean that each
         * has every number from 1 to 9. now we do the more intensive task of
         * searching for duplicate numbers... */

        /* initialize a new UArray that will keep track of the existance of 
         * each number (1 to 9) in each row, col, and box. */
        UArray_T tracker = UArray_new(9, sizeof(bool));
        for (int i = 0; i < UArray_length(tracker); i++) {
                bool *p = UArray_at(tracker, i);
                *p = false;
        }
        UArray_T *tracker_p = &tracker;

        /* call helpers to detect duplicates in each row, col, and box */
        UArray2_map_row_major(array, check_row_duplicates, tracker_p);
        UArray2_map_col_major(array, check_col_duplicates, tracker_p);
        check_box_duplicates(array, tracker_p);

        /* clean up memory before exiting */
        Pnmrdr_free(&image);
        UArray2_free(&array);
        UArray_free(tracker_p);
        FREE(sum);

        exit(0); /* if we've reached the end of this function, the puzzle is
                  * a valid solution. */
}

void add_row(int row, int col, UArray2_T array, void *cl, void *sum)
{
        /* compiler makes us use all the variables */
        (void)cl;

        int *sum_temp = sum;  /* need to make sum an integer so we can 
                               * do arithmetic. */

        int *num = UArray2_at(array, row, col);
        *sum_temp += *num;  /* add current number to sum of row */

        if (row == (UArray2_width(array) - 1)) {  /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * row. */
                if (*sum_temp != 45) {
                        UArray2_free(&array);
                        FREE(sum);
                    printf("NOT CORRECT: ADD_ROW\n");
                        exit(1);      /* row doesn't add up to 45, puzzle 
                                       * is broken. */
                } else {
                        /* row is valid, reset the sum for the next 
                         * iteration and continue. */
                       *sum_temp = 0;
                        return;
                }
                FREE(sum_temp);
        }
}

void add_col(int row, int col, UArray2_T array, void *cl, void *sum)
{
        /* compiler makes us use all the variables */
        (void)cl;

        int *sum_temp = sum;  /* need to make sum an integer so we can 
                               * do arithmetic. */

        int *num = UArray2_at(array, row, col);
        *sum_temp += *num;  /* add current number to sum of column */

        if (col == (UArray2_height(array) - 1)) { /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * column. */
                if (*sum_temp != 45) {
                        UArray2_free(&array);
                        FREE(sum);
                        FREE(sum_temp);
                    printf("NOT CORRECT: ADD_COL\n");
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
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {    /* this will loop nine times,
                                                  * one for each box. */
                        *sum = 0; /* reset sum before each box */

                        for (int row = 0; row < 3; row++) {
                                for (int col = 0; col < 3; col++) {
                                        /* loop within each box row-by-row */
                                        int *num = UArray2_at(array, 
                                                                (3 * i) + row,
                                                                (3 * j) + col);
                                        /* find the row and column of each 
                                         * number in the context of the entire
                                         * puzzle using i and j as offsets. */

                                        /* add number to sum */
                                        *sum += *num; 
                                }
                        }
                }
        }

        if (*sum != 45) {
                UArray2_free(&array);
                FREE(sum);
                    printf("NOT CORRECT: ADD_BOX\n");
                exit(1);   /* box doesn't add up to 45, puzzle is broken. */
        } else {
                *sum = 0;  /* box is valid, reset the sum for the next 
                            * iteration and continue. */
                return;
        }
}

void check_row_duplicates(int row, int col, UArray2_T array, void *cl, 
                                                             void *tracker)
{
        /* compiler makes us use all the variables */
        (void)cl;

        UArray_T *t = tracker;
        int *num = UArray2_at(array, row, col);

        /* switch the existance of this number in the array to true */
        int index = *num - 1;
        bool *num_bool = UArray_at(*t, index);
        *num_bool = true;

        if (row == (UArray2_width(array) - 1)) {  /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * row. */
                for (int i = 0; i < UArray_length(*t); i++) {
                        /* if we find one number that wasn't in this row, 
                         * this is an invalid puzzle. */
                        bool *temp = UArray_at(*t, i);
                        if (*temp == false) {
                                UArray2_free(&array);
                                FREE(tracker);
                                exit(1);
                        }
                }

                for (int i=0; i<UArray_length(*t); i++) {
                        bool *temp = UArray_at(*t,i);
                        *temp = false;
                }
        }
}

void check_col_duplicates(int row, int col, UArray2_T array, void *cl, 
                                                             void *tracker)
{
        /* compiler makes us use all the variables */
        (void)cl;

        UArray_T *t = tracker;
        int *num = UArray2_at(array, row, col);

        /* switch the existance of this number in the array to true */
        int index = *num - 1;
        bool *num_bool = UArray_at(*t, index);
        *num_bool = true;

        if (col == (UArray2_height(array) - 1)) { /* if we're looking at the 
                                                   * last (ninth) number in the
                                                   * column. */
                for (int i = 0; i < UArray_length(*t); i++) {
                        /* if we find one number that wasn't in this column, 
                         * this is an invalid puzzle. */
                        bool *temp = UArray_at(*t, i);
                        if (*temp == false) {
                                UArray2_free(&array);
                                FREE(tracker);
                                exit(1);
                        }
                }

                /* reset the tracker array for the next column. */
                for (int i=0; i<UArray_length(*t); i++) {
                        bool *temp = UArray_at(*t,i);
                        *temp = false;
                }
        }
}

void check_box_duplicates(UArray2_T array, void *tracker)
{
        UArray_T *t = tracker;

        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {    /* this will loop nine times,
                                                  * one for each box. */

                        for (int i=0; i<UArray_length(*t); i++) {
                                bool *temp = UArray_at(*t,i);
                                *temp = false;
                        }                                 /* reset the tracker
                                                           * array for each 
                                                           * box. */

                        for (int row = 0; row < 3; row++) {
                                for (int col = 0; col < 3; col++) {
                                        /* loop within each box row-by-row */
                                        int *num = UArray2_at(array, 
                                                                (3 * i) + row,
                                                                (3 * j) + col);

                                        /* switch the existance of this number 
                                         * in the array to true */
                                        int index = *num - 1;
                                        bool *num_bool = UArray_at(*t, index);
                                        *num_bool = true;
                                }
                        }
                }
        }

        for (int i = 0; i < UArray_length(*t); i++) {
                /* if we find one number that wasn't in this box, 
                 * this is an invalid puzzle. */
                bool *temp = UArray_at(*t, i);
                if (*temp == false) {
                        UArray2_free(&array);
                        FREE(tracker);
                        exit(1);
                }
        }        
}
