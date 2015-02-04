/*
 *   uarray2.c
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
#include "assert.h"
#include "uarray.h"
#include <uarrayrep.h>
#include "mem.h"
#include <stdio.h>

#define T UArray2_T

struct T {
        int height;
        int width;
        int size;
        UArray_T outer;
};

T UArray2_new(int width, int height, int size)
{
        assert(width > 0);
        assert(height > 0);
        assert(size > 0);

        UArray_T outer = UArray_new(width, sizeof(UArray_T));

        for (int i = 0; i < width; i++) {
                UArray_T inner = UArray_new(height, size);

                UArray_T *inner_temp = UArray_at(outer,i);
                *inner_temp = inner;
        }

        UArray2_T uarray2;
        NEW(uarray2);

        uarray2->width = width;
        uarray2->height = height;
        uarray2->size = size;
        uarray2->outer = outer;

        return uarray2;
}

void UArray2_free(T *uarray2)
{
        assert(uarray2 != NULL);

        UArray_T outer = (*uarray2)->outer;
        for (int i = 0; i < UArray2_width(*uarray2); i++) {
                UArray_free(UArray_at(outer, i));
        }

        UArray_free(&outer);

        FREE(*uarray2);
}

int UArray2_width(T uarray2)
{
        assert(uarray2);

        return uarray2->width;
}

int UArray2_height(T uarray2)
{
        assert(uarray2);

        return uarray2->height;
}

int UArray2_size(T uarray2)
{
        assert(uarray2);

        return uarray2->size;
}

void* UArray2_at(T uarray2, int i, int j)
{
        assert(uarray2);
        assert(i >= 0 && i < uarray2->width);
        assert(j >= 0 && j < uarray2->height);

        UArray_T *inner_temp = UArray_at(uarray2->outer, i);
        return UArray_at(*inner_temp, j);
}

void UArray2_map_row_major(T uarray2, 
                           void apply(int i, int j, UArray2_T a, 
                                      void *local_cl, void *global_cl),
                           void *global_cl)
{
        assert(uarray2);

        for (int i = 0; i < uarray2 -> height; i++) {
                for (int j = 0; j < uarray2 -> width; j++) {
                        void *temp = UArray2_at(uarray2, j, i);
                        apply(j, i, uarray2, temp, global_cl);
                }
        }
}

void UArray2_map_col_major(T uarray2, 
                           void apply(int i, int j, UArray2_T a, 
                                      void *local_cl, void *global_cl),
                           void *global_cl)
{
        assert(uarray2);

        for (int i = 0; i < uarray2 -> width; i++) {
                for (int j = 0; j < uarray2 -> height; j++) {
                        void *temp = UArray2_at(uarray2, i, j);
                        apply(i, j, uarray2, temp, global_cl);
                }
        }
}

#undef T