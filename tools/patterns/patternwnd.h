#pragma once

typedef struct PatternItem
{
    char    Name[128];
    float   Lamda;
    unsigned char * PatternRawImage;
    long    PatternBufferSize;
    long    PatternWidth;
    long    PatternHeight;
    HBITMAP PatternBitmap;
    long    Hidden;
    long    pcount;
    long    ncount;
} PatternItem;

void PatternInit(HWND Parent, char * dbfile);

void PatternResize(int Width, int Height);

void RearrangePatternTiles(void);

void PatternRedraw(void);

PatternItem * PatternGetItem(char * PatternName);

void DrawPattern(PatternItem *Item, HDC hdc, LPRECT Rect, BOOL Flipped, BOOL Mirrored, BOOL Box, BOOL Label, BOOL SelectHint);

void PatternDestroy(void);

void ParseDatabase(char *text);
