#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

extern void compute_brightness(FILE *fp);
extern void check_validity(FILE *fp);

int main(int argc, char *argv[])
{
        if(argc == 1) {    /* no filename, use stdin */
                compute_brightness(stdin);
        } else if (argc == 2) {     /* one filename, use it */
                FILE *fp = fopen(argv[1], "r");

                if(fp == NULL) {
                        fprintf(stderr,
                                "ERROR: Could not open file %s for reading.\n",
                                argv[1]);
                } else {
                        // check_validity(fp); /* NOTE BELOW */
                        compute_brightness(fp);
                        fclose(fp);
                }
        } else {    /* more than one filename, abort */
                fprintf(stderr,
                        "ERROR: Please only give one filename at a time.\n");
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

/* NOTE: We tried very hard to implement error handling - aka showing our own
 * error message if the image isn't in a valid graymap format instead of saying
 * "Uncaught exception" (if the file is a PNG, for example). However, this code 
 * seems to cause more errors than it handles. We had to prioritize and leave
 * this out last minute. Sorry!!!!
 */
extern void check_validity(FILE *fp)
{
        TRY
                Pnmrdr_T image = Pnmrdr_new(fp);
                (void)image; /* compiler makes us use all variables */
        EXCEPT(Pnmrdr_Badformat)
                fprintf(stderr,
                        "ERROR: Data is not a portable grayscale image.\n");
                exit(EXIT_FAILURE);
        END_TRY;       
}