// Horizontal patterns text saver.
// TODO.

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include "jpegwnd.h"

// Just enumerate added patterns layer.
static void DebugEnum(PatternEntry *List, int Count, FILE *f)
{
    int n;

    for (n = 0; n < Count; n++)
    {
        fprintf(f,
            "Pattern %i : name=%s, posx:%i, posy:%i\n",
            n, List[n].PatternName, List[n].PlaneX, List[n].PlaneY);
    }
}

void TextSaverHorz(PatternEntry *List, int Count, char *Filename)
{
    FILE * f;

    if (Count == 0) return;

    //
    // Create output file
    //

    f = fopen(Filename, "wt");

    if (f)
    {
        DebugEnum(List, Count, f);

        fclose(f);
    }
}