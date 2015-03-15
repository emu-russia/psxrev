
typedef struct PatternItem
{
    char    Name[128];
    float   Lamda;
    unsigned char * PatternRawImage;
    int     PatternBufferSize;
    int     PatternWidth;
    int     PatternHeight;
    HBITMAP PatternBitmap;
    bool    Hidden;
} PatternItem;

void PatternInit(HWND Parent, char * dbfile);

void PatternResize(int Width, int Height);

void RearrangePatternTiles(void);

void PatternRedraw(void);

PatternItem * PatternGetItem(char * PatternName);

void DrawPattern(PatternItem *Item, HDC hdc, LPRECT Rect, bool Flipped, bool Box, bool Label);

void PatternDestroy(void);

void ParseDatabase(char *text);

char * GetSavedDatabase(void);
