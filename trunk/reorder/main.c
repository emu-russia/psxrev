// Reorder chip images.

// c0000_r0000.jpg  ->  r0000_c0000.jpg

#include <windows.h>

char * input_path = "C:/Andruxa/PSXCPU/cxd8530cq/lap2_ns50xu_1";
char * output_path = "C:/Andruxa/PSXCPU/cxd8530cq/lap2_ns50xu_2";

main ()
{
    int     column, row;
    char    input[1000], output[1000];

    for (column=0; column<44; column++)
    {
        for (row=0; row<66; row++)
        {
            sprintf (input, "%s/c%04i_r%04i.jpg", input_path, column, row );
            sprintf (output, "%s/r%04i_c%04i.jpg", output_path, row, column);

            //printf ( "%s\n%s\n", input, output ); break;

            CopyFile (input, output, FALSE);
        }
    }

}