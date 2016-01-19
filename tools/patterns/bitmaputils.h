
VOID StretchBitmap ( PUCHAR DestBitmap,
                     PUCHAR SourceBitmap,
                     LONG SourceWidth,
                     LONG SourceHeight,
                     LONG DestWidth,
                     LONG DestHeight,
                     BOOLEAN Flip,
                     BOOLEAN Mirror );

VOID FontPrintf ( PUCHAR DestBitmap,
                  LONG PosX,
                  LONG PosY,
                  LONG FontHeight,
                  PCHAR Format, 
                  ... );
