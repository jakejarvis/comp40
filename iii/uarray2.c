



UArray2 UArray2_new(int width, int height, int size)
{
        UArray_T outer = UArray_new(height, (width * size));

        for(int i = 0; i < width, i++) {
                *UArray_at(outer, i) = UArray_new(width, size);
        }

        return outer;
}

void UArray2_free(T *uarray2)
{
        for(int i = 0; i < UArray2_height(uarray2); i++) {
                UArray_free(&UArray2_at(uarray2, i));
        }

        UArray_free(&uarray2);
}

int UArray2_width(T uarray2)
{

}

int UArray2_height(T uarray2)
{

}

int UArray2_size(T uarray2)
{

}

void UArray2_at(T uarray2, int i, int j)
{

}

void UArray2_map_row_major(T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p)
{

}

void UArray2_map_col_major(T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p)
{

}