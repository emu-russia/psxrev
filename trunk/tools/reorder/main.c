// Reorder chip images.

#include <windows.h>

char * input_path = "C:\\Andruxa\\PSXCPU\\m1_lap2";
char * output_path = "C:\\Andruxa\\PSXCPU\\m1_lap2_reorder";

main ()
{
    int     column, row;    
    char    input[1000], output[1000];

	for (row = 35; row <= 62; row++)
	{
		for (column = 0; column <= 28; column++)
		{
			sprintf(input, "%s/r%04i_c%04i.jpg", input_path, row, column);
			sprintf(output, "%s/r%04i_c%04i.jpg", output_path, column, row-35);

			CopyFile(input, output, FALSE);
		}
	}
}