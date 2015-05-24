
//
// Enable OpenGL renderer
//

#define USEGL

typedef struct PatternEntry
{
    char    PatternName[64];

    union
    {
        struct      // GL Context
        {
            unsigned char * TextureBuffer;
            unsigned int TextureId;
        };
        char    Reserved[64];
    };

    long    PosX;           // Relative to upper-left corner of parent window
    long    PosY;
    long    SavedPosX;      // Old position saved here during scrolling
    long    SavedPosY;
    long    PlaneX;         // Relative to upper-left corner of source image
    long    PlaneY;
    long    Width;
    long    Height;
    union
    {
        HWND    Hwnd;
        unsigned __int64 Padding;
    };
    long    Flipped;
    float   BlendLevel;     // UpdateLayeredWindow
} PatternEntry;

void JpegInit(HWND Parent);

void JpegLoadImage(char *filename, bool Silent);

bool JpegGetSelectRegion(LPRECT Region);

void JpegSetSelectRegion(LPRECT Region);

void JpegResize(int Width, int Height);

int JpegWindowWidth(void);

int JpegWindowHeight(void);

void AddPatternEntry(char * PatternName);

void UpdatePatternEntry(int EntryIndex, PatternEntry * Entry);

void JpegRemoveSelection(void);

void JpegRedraw(void);

PatternEntry * GetPatternEntry(int EntryIndex);

int GetPatternEntryNum(void);

void JpegDestroy(void);

void JpegRemoveAllPatterns(void);

char * JpegGetImageName(bool NameOnly);

void JpegGetScroll(LPPOINT Offset);

void JpegSetScroll(LPPOINT Offset);

void JpegSelectPattern(PatternEntry * Pattern);

PatternEntry * JpegGetSelectedPattern(void);

void JpegEnsureVisible(PatternEntry * Pattern);
