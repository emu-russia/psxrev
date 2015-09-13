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
} PatternItem;

void PatternInit(HWND Parent, char * dbfile);

void PatternResize(int Width, int Height);

void RearrangePatternTiles(void);

void PatternRedraw(void);

PatternItem * PatternGetItem(char * PatternName);

void DrawPattern(PatternItem *Item, HDC hdc, LPRECT Rect, bool Flipped, bool Mirrored, bool Box, bool Label, bool SelectHint);

void PatternDestroy(void);

void ParseDatabase(char *text);

char * GetSavedDatabase(void);
