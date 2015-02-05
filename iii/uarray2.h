/*
 *   uarray2.h
 *
 *   Jake Jarvis (jjarvi01)
 *   Tam Luong   (tluong04)
 *
 *   COMP 40
 *   Homework 2
 *   2/3/15
 *
 */

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
                     void apply(int i, int j, UArray2_T a, void *local_cl, 
                                                           void *global_cl),
                     void *global_cl);
extern void     UArray2_map_col_major   (T uarray2, 
                     void apply(int i, int j, UArray2_T a, void *local_cl, 
                                                           void *global_cl),
                     void *global_cl);

#undef T
#endif
