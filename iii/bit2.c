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
        UArray_T outer;
};



T Bit2_new(int width, int height)
{
        assert(width > 0);
        assert(height > 0);


        UArray_T outer = UArray_new(width, sizeof(UArray_T));

        for(int i = 0; i < width; i++) {
		Bit_T bit = Bit_new(height);
                Bit_T *bit_temp = UArray_at(outer, i);
	    	*bit_temp = bit;
        }

        Bit2_T bitmap;
        NEW(bitmap);

        bitmap->width = width;
        bitmap->height = height;
        bitmap->outer = outer;

        return bitmap;
}

void Bit2_free(T *bitmap)
{
        assert(bitmap != NULL);
	assert(*bitmap != NULL);
	for (int i=0; i < (*bitmap)->width; i++) {
		Bit_T *bit_temp = UArray_at((*bitmap)->outer,i);
		Bit_free(bit_temp);
	}
	UArray_free(&(*bitmap)->outer);
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
    	assert(bitmap);
    	assert(0 <= i && i < bitmap->width);
	assert(0 <= j && j < bitmap->height);
	UArray_T outer = bitmap->outer;
	Bit_T *bit_temp = UArray_at(outer,i);
        return Bit_put(*bit_temp, j, bit);
}

int Bit2_get(T bitmap, int i, int j)
{
	assert(bitmap);
	assert(0 <= i && i < bitmap->width);
	assert(0 <= j && j < bitmap->height);
	UArray_T outer = bitmap->outer;
	Bit_T *bit_temp = UArray_at(outer,i);
        return Bit_get(*bit_temp, j);
}

void Bit2_map_row_major(T bitmap, 
                        void apply(int i, int j, T bitmap, int bit, void *cl),
                        void *cl)
{
        for(int i = 0; i < bitmap->height; i++) {
                for(int j = 0; j < bitmap->width; j++) {
			int temp = Bit2_get(bitmap, j, i);
                        apply(j, i, bitmap, temp, cl);
                }
        }
}

void Bit2_map_col_major(T bitmap, 
                        void apply(int i, int j, T bitmap, int bit, void *cl),
                        void *cl)
{
        for(int i = 0; i < bitmap->width; i++) {
                for(int j = 0; j < bitmap->height; j++) {
			int temp = Bit2_get(bitmap, i, j);
                        apply(i, j, bitmap, temp, cl);
                }
        }
}


#undef T