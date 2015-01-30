#include "uarray2.h"
#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include "uarray.h"

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


        UArray_T outer = UArray_new(height, (width * size));

        UArray2_T uarray2;
        NEW(uarray2);

        uarray2->width = width;
        uarray2->height = height;
        uarray2->size = size;
        uarray2->outer = outer;

        for(int i = 0; i < width; i++) {
                UArray_T *p = UArray_at(outer, i);
                *p = UArray_new(width, size);
        }

        return uarray2;
}

void UArray2_free(T uarray2)
{
        for(int i = 0; i < UArray2_height(uarray2); i++) {
                UArray_free(UArray_at((uarray2->outer), i));
        }

        UArray_free(&(uarray2->outer));
}

int UArray2_width(T uarray2)
{
        return uarray2->width;
}

int UArray2_height(T uarray2)
{
        return uarray2->height;
}

int UArray2_size(T uarray2)
{
        return uarray2->size;
}

void* UArray2_at(T uarray2, int i, int j)
{
        return UArray_at(UArray_at((uarray2->outer), i), j);
}

void UArray2_map_row_major(T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p)
{
        for(int i = 0; i < uarray2 -> height; i++) {
                for (int j = 0; j < uarray2 -> width; j++) {
                        apply(i, j, uarray2, p, NULL);    // **** TO-DO: P1 AND P1
                }
        }
}

void UArray2_map_col_major(T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p)
{
        for(int i = 0; i < uarray2 -> width; i++) {
                for (int j = 0; j < uarray2 -> height; j++) {
                        apply(i, j, uarray2, p, NULL);    // **** TO-DO: P1 AND P1
                }
        }
}