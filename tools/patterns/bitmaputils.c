// Stretching of RGB bitmap data and font drawing.

#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

static BOOLEAN FontInit = FALSE;

VOID StretchBitmap ( PUCHAR DestBitmap,
                     PUCHAR SourceBitmap,
                     LONG SourceWidth,
                     LONG SourceHeight,
                     LONG DestWidth,
                     LONG DestHeight,
                     BOOLEAN Flip,
                     BOOLEAN Mirror )
{
}

VOID FontPrintf ( PUCHAR DestBitmap,
                  LONG PosX,
                  LONG PosY,
                  LONG FontHeight,
                  PCHAR Format, 
                  ... )
{
    va_list Arg;
    int Length;
    CHAR Buffer[0x1000];

    //
    // Init font bitmap data
    //

    if ( !FontInit )
    {


        FontInit = TRUE;
    }

    va_start(Arg, Format);
    Length = vsprintf(Buffer, Format, Arg);
    va_end(Arg);
}
