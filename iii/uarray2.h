
#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;


extern T        UArray2_new     (int width, int height, int size);

extern void     UArray2_free    (T *uarray2);

extern int      UArray2_width   (T uarray2);
extern int      UArray2_height  (T uarray2);
extern int      UArray2_size    (T uarray2);

extern void    *UArray2_at      (T uarray2, int i, int j);

extern void     UArray2_map_row_major   (T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p);
extern void     UArray2_map_col_major   (T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *p1, void *p2),
                     void *p);

#undef T

#endif