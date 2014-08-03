// Reorder chip images.

#include <windows.h>

char * input_path = "C:\\Andruxa\\PSXCPU\\m1_lap1";
char * output_path = "C:\\Andruxa\\PSXCPU\\m1_lap1_reorder";

main ()
{
    int     column, row;    
    char    input[1000], output[1000];

	for (column = 0; column <= 30; column++)
	{
		for (row = 0; row <= 68; row++)
		{
			sprintf(input, "%s/c%04i_r%04i.jpg", input_path, column, 68-row);
			sprintf(output, "%s/c%04i_r%04i.jpg", output_path, column, row);

			CopyFile(input, output, FALSE);
		}
	}
}