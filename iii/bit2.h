/*
 *   bit2.h
 *
 *   Jake Jarvis (jjarvi01)
 *   Tam Luong   (tluong04)
 *
 *   COMP 40
 *   Homework 2
 *   2/3/15
 *
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define T Bit2_T
typedef struct T *T;

extern T        Bit2_new    (int width, int height);

extern void     Bit2_free   (T *bitmap);

extern int      Bit2_width  (T bitmap);
extern int      Bit2_height (T bitmap);

extern int      Bit2_put    (T bitmap, int i, int j, int bit);
extern int      Bit2_get    (T bitmap, int i, int j);

extern void     Bit2_map_row_major  (T bitmap, 
                         void apply(int i, int j, T bitmap, int bit, void *cl),
                         void *cl);
extern void     Bit2_map_col_major  (T bitmap, 
                         void apply(int i, int j, T bitmap, int bit, void *cl),
                         void *cl);

#undef T
#endif