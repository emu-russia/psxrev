
/*
Selected: 100 / 100px, 3.4 / 5.5l / ---
Scroll : 100 / 100px
Patterns : Shown / 100
Patterns Added : 5
Lamda / Delta : 6.0 / 3.0
Source Image : Path / Not Loaded(last)
*/

// Status Bar Parts
enum {
    STATUS_SELECTED = 1,
    STATUS_SCROLL,
    STATUS_PATTERNS,
    STATUS_ADDED,
    STATUS_LAMDA_DELTA,
    STATUS_SOURCE_IMAGE,
};

void CreateStatusBar(HWND Parent);

void SetStatusText(int sbPart, char *text);

int GetStatusBarHeight(void);

void ResizeStatusBar(int Width, int Height);
