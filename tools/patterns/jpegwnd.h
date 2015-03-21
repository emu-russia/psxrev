
typedef struct PatternEntry
{
    char    PatternName[128];
    int     PosX;           // Относительно верхнего левого угла родительского окна.
    int     PosY;
    int     SavedPosX;      // Старое положение окна сохраняется сюда при скроллинге
    int     SavedPosY;
    int     PlaneX;         // Относительно верхнего левого угла исходной картинки.
    int     PlaneY;
    int     Width;
    int     Height;
    HWND    Hwnd;
    bool    Flipped;
    float   BlendLevel;     // UpdateLayeredWindow
} PatternEntry;

void JpegInit(HWND Parent);

void JpegLoadImage(char *filename, bool Silent);

bool JpegGetSelectRegion(LPRECT Region);

void JpegSetSelectRegion(LPRECT Region);

void JpegResize(int Width, int Height);

int JpegWindowWidth(void);

void AddPatternEntry(char * PatternName);

void UpdatePatternEntry(int EntryIndex, PatternEntry * Entry);

void JpegRemoveSelection(void);

void JpegRedraw(void);

PatternEntry * GetPatternEntry(int EntryIndex);

int GetPatternEntryNum(void);

void JpegDestroy(void);

void JpegRemoveAllPatterns(void);

char * JpegGetImageName(void);

void JpegGetScroll(LPPOINT Offset);

void JpegSetScroll(LPPOINT Offset);

void JpegSelectPattern(PatternEntry * Pattern);

PatternEntry * JpegGetSelectedPattern(void);

void JpegEnsureVisible(PatternEntry * Pattern);
