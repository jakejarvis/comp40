#include <stdio.h>
#include <pnmrdr.h>

extern void compute_brightness(FILE *fp);

int main(int argc, char *argv[])
{
        if(argc == 1) {
                compute_brightness(stdin);
        } else {
                for(int i = 1; i < argc; i++) {
                        FILE *fp = fopen(argv[i], "r");

                        if(fp == NULL) {
                                fprintf(stderr,
                                        "Could not open file %s for reading\n",
                                        argv[i]);
                        } else {
                                compute_brightness(fp);
                                fclose(fp);
                        }
                }
        }
        
        return 0;
}

extern void compute_brightness(FILE *fp)
{
        Pnmrdr_T image = Pnmrdr_new(fp);
        Pnmrdr_mapdata map = Pnmrdr_data(image);
        double sum = 0;
        
        for(unsigned int i = 0; i < (map.width * map.height); i++) {
                sum += Pnmrdr_get(image);
        }
        
        double average = sum / (map.width * map.height * map.denominator);
        
        printf("%.3f\n", average);       
}
