// Reorder chip images.

#include <windows.h>

char * input_path = "D:\\Andruxa\\PSXCPU\\lap2_reorder";
char * output_path = "D:\\Andruxa\\PSXCPU\\lap2_2";

main ()
{
    int     column, row;
    
    char    input[1000], output[1000];

    for (column=0; column<44; column++)
    {
        for (row=0; row<66; row++)
        {
//            sprintf (input, "%s/c%04i_r%04i.jpg", input_path, column, row );
//            sprintf (output, "%s/r%04i_c%04i.jpg", output_path, row, column);

            sprintf (input, "%s/r%04i_c%04i.jpg", input_path, row, column  );
            sprintf (output, "%s/r%04i_c%04i.jpg", output_path, 65-row, 43-column );

            //printf ( "%s\n%s\n", input, output ); break;

            CopyFile (input, output, FALSE);
        }
    }

}