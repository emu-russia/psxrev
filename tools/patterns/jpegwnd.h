
void JpegInit(HWND Parent);

void JpegLoadImage(char *filename);

bool JpegGetSelectRegion(LPRECT Region);

void JpegResize(int Width, int Height);

int JpegWindowWidth(void);

void AddPatternEntry(int PatternIndex);

void JpegRemoveSelection(void);

void JpegRedraw(void);
