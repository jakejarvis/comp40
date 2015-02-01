#include "bit2.h"
#include "uarray.h"
#include <stdlib.h>
#include "bit.h"
#include "assert.h"
#include "mem.h"

#define T Bit2_T

struct T {
        int width;
        int height;
        UArray_T array;
};



T Bit2_new(int width, int height)
{
        assert(width > 0);
        assert(height > 0);



        UArray_T array = UArray_new(width, sizeof(Bit_new(height)));
/*
        for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                        Bit_T *bit = UArray2_at(array, i, j);
                        bit = Bit_new(1);
                }
        }
*/


        for(int i = 0; i < width; i++) {
                Bit_T *bit = UArray_at(array, i);
                *bit = Bit_new(height);
        }



        Bit2_T bitmap;
        NEW(bitmap);

        bitmap->width = width;
        bitmap->height = height;
        bitmap->array = array;

        return bitmap;
}

void Bit2_free(T *bitmap)
{
        assert(bitmap);

        UArray_free(&((*bitmap)->array));



        FREE(*bitmap);
}

int Bit2_width(T bitmap)
{
        assert(bitmap);
        return bitmap->width;
}

int Bit2_height(T bitmap)
{
        assert(bitmap);
        return bitmap->height;
}

int Bit2_put(T bitmap, int i, int j, int bit)
{
        return Bit_put(UArray_at(bitmap->array, i), j, bit);


     //   Bit_T *bit = UArray2_at(bitmap->array, i, j);
//  *bit = p;
}

int Bit2_get(T bitmap, int i, int j)
{
        return Bit_get(UArray_at(bitmap->array, i), j);
     //   Bit_T *bit = UArray2_at(bitmap->array, i, j);
     //   return bit;
}

void Bit2_map_row_major(T bitmap, 
                        void apply(int i, int j, T bitmap, int bit, void *cl),
                        void *cl)
{
//        return Array2_map_row_major()

        for(int i = 0; i < bitmap->height; i++) {
                for(int j = 0; j < bitmap->width; j++) {
                        apply(i, j, bitmap, Bit2_get(bitmap, i, j), cl);
                }
        }

//                Bit_map(  UArray_at(bitmap->array, i), apply(i, bit, cl), cl  );
}

void Bit2_map_col_major(T bitmap, 
                        void apply(int i, int j, T bitmap, int bit, void *cl),
                        void *cl)
{
        for(int i = 0; i < bitmap->width; i++) {
                for(int j = 0; j < bitmap->height; j++) {
                        apply(i, j, bitmap, Bit2_get(bitmap, i, j), cl);
                }
        }
}


#undef T