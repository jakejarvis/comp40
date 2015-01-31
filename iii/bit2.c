#include "bit2.h"
#include "uarray2.h"
#include <stdlib.h>
#include "assert.h"
#include "mem.h"

#define T Bit2_T

struct T {
  int height;
  int width;
  //Array2_T array;
};

/*

T Bit2_new (int width, int height)
{

}

void Bit2_free (T *bitmap)
{

}

void Bit2_put (T bitmap, int i, int j, void *p)
{

}

void* Bit2_get (T bitmap, int i, int j)
{

}

void Bit2_map_row_major (T bitmap, 
                         void apply(int i, int j, Bit2_T a, int b, void *p1),
                         void *p)
{

}

void Bit2_map_col_major (T bitmap, 
                         void apply(int i, int j, Bit2_T a, int b, void *p1),
                         void *p)
{
        
}
*/

#undef T