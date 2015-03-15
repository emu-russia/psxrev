#define _CRT_SECURE_NO_WARNINGS

// Окно для отображения паттернов.

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <Windows.h>
#include <windowsx.h>

#include "jpegwnd.h"
#include "jpegload.h"
#include "statuswnd.h"
#include "patternwnd.h"

#define FLIP_BUTTON_HEIGHT  30

static HWND ParentWnd;
static HWND PatternWnd;

extern HWND FlipWnd;
extern float WorkspaceLamda, WorkspaceLamdaDelta;
extern char CurrentWorkingDir[MAX_PATH];

static PatternItem * Patterns = NULL;
static int NumPatterns;

static HWND *PatternTiles;

#define PATTERN_TILE_CLASS "PatternTile"

static char * SavedDatabase = NULL;

static HGDIOBJ PatternFont;

// DEBUG
static void DumpPatterns(void)
{
    char Buffer[0x10000], *ptr = Buffer;
    unsigned n;

    for (n = 0; n < NumPatterns; n++)
    {
        ptr += sprintf(ptr, "pattern %i: %s, lamda:%.1f\n", n, Patterns[n].Name, Patterns[n].Lamda );
    }

    MessageBox(0, Buffer, "Patterns Dump", MB_OK);
}

int GetPatternIndexByHwnd(HWND hwnd)
{
    unsigned n;
    for (n = 0; n < NumPatterns; n++)
    {
        if (PatternTiles[n] == hwnd) return n;
    }
    return -1;
}

void DrawPattern(PatternItem *Item, HDC hdc, LPRECT Rect, bool Flipped, bool Box, bool Label)
{
    HGDIOBJ oldBitmap;
    HDC hdcMem;
    BITMAP bitmap;
    BLENDFUNCTION Blend;
    HPEN Pen;
    HGDIOBJ OldPen;

    if (Item)
    {
        //
        // Draw pattern's images
        //

        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, Item->PatternBitmap);
        GetObject(Item->PatternBitmap, sizeof(bitmap), &bitmap);

        SetStretchBltMode(hdc, HALFTONE);
        if (Flipped)
        {
            StretchBlt(hdc, Rect->right, Rect->bottom, -Rect->right, -Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
        }
        else
        {
            StretchBlt(hdc, Rect->left, Rect->top, Rect->right, Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
        }

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

        //
        // Bounding box
        //

        if (Box)
        {
            #define LINE_WIDTH 2

            Pen = CreatePen(PS_SOLID, LINE_WIDTH, RGB(0xff, 0, 0));
            OldPen = SelectObject(hdc, Pen);
            SetPolyFillMode(hdc, TRANSPARENT);

            //
            // Fuck it....
            //

            MoveToEx(hdc, LINE_WIDTH, LINE_WIDTH, NULL);
            LineTo(hdc, Item->PatternWidth, 0);
            LineTo(hdc, Item->PatternWidth, Item->PatternHeight);
            LineTo(hdc, 0, Item->PatternHeight);
            LineTo(hdc, 0, 0);

            SelectObject(hdc, OldPen);
            DeleteObject(Pen);
        }

        //
        // Draw pattern label
        //

        if (Label)
        {
            SelectObject(hdc, PatternFont);
            TextOut(hdc, 0, 0, Item->Name, strlen(Item->Name));
        }
    }
    else
    {
        SelectObject(hdc, PatternFont);
        TextOut(hdc, 0, 0, "UNKNOWN", strlen("UNKNOWN"));
    }
}

LRESULT CALLBACK PatternTileProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT Rect;
    int PatternIndex;
    PatternItem *Item;
    bool Flipped;

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        PatternIndex = GetPatternIndexByHwnd(hwnd);
        if (PatternIndex != -1)
        {
            Item = &Patterns[PatternIndex];
            //MessageBox(0, Item->Name, "Clicked", 0);
            AddPatternEntry(Item->Name);
            JpegRemoveSelection();
            RearrangePatternTiles();
            PatternRedraw();
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &Rect);

        PatternIndex = GetPatternIndexByHwnd(hwnd);
        if (PatternIndex != -1)
        {
            Item = &Patterns[PatternIndex];
            Flipped = Button_GetCheck(FlipWnd) == BST_CHECKED;
            if (!Item->Hidden) DrawPattern(Item, hdc, &Rect, Flipped, FALSE, FALSE);
        }

        EndPaint(hwnd, &ps);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

bool CheckHidden(int PatternIndex)
{
    PatternItem * Item = &Patterns[PatternIndex];
    RECT Region;
    bool RegionSelected = JpegGetSelectRegion(&Region);
    bool Fit;

    float RegionWidth, RegionHeight;
    float LamdaWidth, LamdaHeight;
    float PLamdaWidth, PLamdaHeight;

    if (RegionSelected)
    {
        //
        // Calculate select box width and height in lamdas
        //

        RegionWidth = (float)abs(Region.right - Region.left);
        RegionHeight = (float)abs(Region.bottom - Region.top);
        LamdaWidth = RegionWidth / WorkspaceLamda;
        LamdaHeight = RegionHeight / WorkspaceLamda;

        //
        // Calculate pattern width and height in lamdas
        //

        PLamdaWidth = (float)Item->PatternWidth / Item->Lamda;
        PLamdaHeight = (float)Item->PatternHeight / Item->Lamda;

        //
        // Match dimensions according to lamda delta
        //

        Fit = false;
        if (PLamdaWidth >= (LamdaWidth - 2 * WorkspaceLamdaDelta) && PLamdaWidth < (LamdaWidth + 2 * WorkspaceLamdaDelta))
        {
            if (PLamdaHeight >= (LamdaHeight - 2 * WorkspaceLamdaDelta) && PLamdaHeight < (LamdaHeight + 2 * WorkspaceLamdaDelta)) Fit = true;
        }
        return !Fit;
    }
    else return false;
}

// Составить из дочерних окон паттернов подобие thumbnail-галереи, путём задания из координат.
// Вызывается при:
//   - Изменении размеров родительского окна
//   - При нажатии на flip
//   - При добавлении нового паттерна из библиотеки
void RearrangePatternTiles(void)
{
    unsigned n;
    RECT Rect;

    GetClientRect(PatternWnd, &Rect);

    char Text[0x100];
    int Border = 2;
    int Width = Rect.right;
    int TileWidth = Width / 3 - (Border << 1);
    int TileHeight;
    float Aspect;
    int HorPos, VerPos = Border;
    int ColCount = 0, RowCount = 0;
    int MaxHeight = 0;
    int ShownPatterns = 0;

    for (n = 0; n < NumPatterns; n++)
    {
        if (ColCount == 0) HorPos = Border;

        Aspect = (float)Patterns[n].PatternWidth / (float)TileWidth;
        TileHeight = floor ( (float)Patterns[n].PatternHeight / Aspect );

        if (CheckHidden(n))
        {
            Patterns[n].Hidden = true;
            MoveWindow(PatternTiles[n], 0, 0, 0, 0, TRUE);
            continue;
        }
        else
        {
            Patterns[n].Hidden = false;
            MoveWindow(PatternTiles[n], HorPos, VerPos, TileWidth, TileHeight, TRUE);
            ShownPatterns++;
        }

        if (TileHeight > MaxHeight) MaxHeight = TileHeight;

        HorPos += TileWidth + (Border << 1);

        ColCount++;
        if (ColCount > 2)
        {
            ColCount = 0;
            RowCount++;
            VerPos += MaxHeight + Border;
            MaxHeight = 0;
        }
    }

    sprintf(Text, "Patterns : %i / %i", ShownPatterns, NumPatterns);
    SetStatusText(STATUS_PATTERNS, Text);

    SetScrollPos(PatternWnd, SB_VERT, 0, TRUE);
}

static void PatternAddScanline(unsigned char *buffer, int stride, void *Param)
{
    PatternItem *Item = (PatternItem *)Param;
    memcpy((void *)(Item->PatternRawImage + Item->PatternBufferSize), buffer, stride);
    Item->PatternBufferSize += stride;
}

// Add new pattern.
void AddNewPattern(char *name, char *jpeg_path, float lamda)
{
    PatternItem Item;
    char buffer[1024];
    int DecodeResult;
    char Text[1024];
    char FullPath[MAX_PATH];

    memset(&Item, 0, sizeof(PatternItem));

    strncpy(Item.Name, name, sizeof(Item.Name) - 1);
    Item.Lamda = lamda;
    Item.Hidden = false;

    sprintf(FullPath, "%s\\%s", CurrentWorkingDir, jpeg_path);

    DecodeResult = JpegLoad(
        FullPath,
        PatternAddScanline,
        &Item.PatternRawImage,
        &Item.PatternBufferSize,
        &Item.PatternWidth,
        &Item.PatternHeight,
        &Item);

    if (DecodeResult == 0)
    {
        sprintf(Text, "Error, decoding pattern JPEG %s", FullPath);
        MessageBox(0, Text, "Error", 0);
        return;
    }

    Item.PatternBitmap = CreateBitmapFromPixels(GetWindowDC(PatternWnd), Item.PatternWidth, Item.PatternHeight, 24, Item.PatternRawImage);

    Patterns = (PatternItem *)realloc(Patterns, sizeof(PatternItem) * (NumPatterns + 1));
    Patterns[NumPatterns] = Item;

    //
    // Create new pattern window
    //

    PatternTiles = (HWND *)realloc(PatternTiles, sizeof(HWND)* (NumPatterns + 1));

    PatternTiles[NumPatterns] = CreateWindowEx(
        0,
        PATTERN_TILE_CLASS,
        "PatternTilePopup",
        WS_OVERLAPPED | WS_CHILDWINDOW,
        2,
        2,
        100,
        100,
        PatternWnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(PatternTiles[NumPatterns], SW_NORMAL);
    UpdateWindow(PatternTiles[NumPatterns]);

    NumPatterns++;

    RearrangePatternTiles();

    // DEBUG
    //sprintf(buffer, "Add Pattern: name:%s, lamda:%f, path:[%s]\n", name, lamda, jpeg_path);
    //MessageBox(0, buffer, "New Pattern", 0);
}

// Trim leading and trailing whitespaces and quotes in given string.
char *TrimString(char *str)
{
    int length, n;
    while (*str == ' ' || *str == '\"') str++;
    length = strlen(str);
    for (n = length - 1; n > 0; n--)
    {
        if (str[n] == ' ' || str[n] == '\"') str[n] = 0;
        else break;
    }
    return str;
}

char *trimwhitespace(char *str)
{
    static char *WHITESPACE = " \t\n\r\"";
    int spacesAtStart = strspn(str, WHITESPACE);
    char *result = str + spacesAtStart;
    int lengthOfNonSpace = strcspn(result, WHITESPACE);
    result[lengthOfNonSpace] = 0;
    return result;
}

void ParseLine(char *line)
{
    char c;
    char command[32];
    char name[128];
    char lamda[16];
    char path[256];
    char buffer[1024];

    // Skip whitespaces.
    while (*line)
    {
        c = *line;
        if (c > 0 && c <= ' ') line++;
        else break;
    }

    // Check empty string.
    if (strlen(line) == 0) return;

    // Check comments
    if (*line == '#') return;

    sscanf(line, "%s %[^','],%[^','],%s", command, name, lamda, path);

    if (!_stricmp(command, "pattern"))
    {
        AddNewPattern(TrimString(name), TrimString(path), (float)atof(lamda));
    }
    
    // DEBUG.
    //sprintf(buffer, "Case: command:%s, name:%s, lamda:%s, path:%s\n", command, name, lamda, path);
    //MessageBox(0, buffer, "Line", 0);
}

void ParseDatabase(char *text)
{
    char Text[0x100];
    char linebuffer[0x10000], *ptr = text;
    char *lineptr = linebuffer, c = *ptr;
    
    // Сохраним базу данных текущей рабочей среды.
    if (SavedDatabase)
    {
        free(SavedDatabase);
        SavedDatabase = NULL;
    }
    SavedDatabase = (char *)malloc(strlen(text) + 1);
    strcpy(SavedDatabase, text);
    SavedDatabase[strlen(text)] = 0;

    // Загружаем по строкам.
    while (c)
    {
        c = *ptr++;
        if (c == '\n' || c == 0)
        {
            *lineptr++ = 0;
            ParseLine(linebuffer);
            lineptr = linebuffer;
        }
        else *lineptr++ = c;
    }

    sprintf(Text, "Patterns : %i / %i", NumPatterns, NumPatterns);
    SetStatusText(STATUS_PATTERNS, Text);
}

LRESULT CALLBACK PatternProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    RECT Rect;
    RECT ScrollRect;
    int ScrollBarWidth;
    SCROLLINFO si;
    int yChar;
    int yPos;

    yChar = 1;

    switch (msg)
    {
    case WM_CREATE:
        SetScrollRange(hwnd, SB_VERT, 0, 10000, TRUE);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        break;

    case WM_VSCROLL:
        // Get all the vertial scroll bar information.
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        // Save the position for comparison later on.
        yPos = si.nPos;
        switch (LOWORD(wParam))
        {

            // User clicked the HOME keyboard key.
        case SB_TOP:
            si.nPos = si.nMin;
            break;

            // User clicked the END keyboard key.
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;

            // User clicked the top arrow.
        case SB_LINEUP:
            si.nPos -= 1;
            break;

            // User clicked the bottom arrow.
        case SB_LINEDOWN:
            si.nPos += 1;
            break;

            // User clicked the scroll bar shaft above the scroll box.
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;

            // User clicked the scroll bar shaft below the scroll box.
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;

            // User dragged the scroll box.
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;

        default:
            break;
        }

        // Set the position and then retrieve it.  Due to adjustments
        // by Windows it may not be the same as the value set.
        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hwnd, SB_VERT, &si);

        // If the position has changed, scroll window and update it.
        if (si.nPos != yPos)
        {
            ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
            UpdateWindow(hwnd);
        }
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void PatternInit(HWND Parent, char * dbfile)
{
    FILE *f;
    WNDCLASSEX wc;
    char *buffer;
    int filesize;
    RECT Rect;
    int ScrollBarWidth;
    LOGFONT LogFont;

    ParentWnd = Parent;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = PatternProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "PatternWnd";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Cannot register PatternWnd Class", "Error", 0);
        return;
    }

    GetClientRect(ParentWnd, &Rect);

    PatternWnd = CreateWindowEx(
        0,
        "PatternWnd",
        "PatternWndPopup",
        WS_OVERLAPPED | WS_CHILDWINDOW | WS_VSCROLL,
        2,
        2,
        Rect.right,
        Rect.bottom,
        ParentWnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(PatternWnd, SW_NORMAL);
    UpdateWindow(PatternWnd);

    //
    // Создадим галочку "flip"
    //

    FlipWnd = CreateWindow(
        "BUTTON", 
        "flip", 
        BS_CHECKBOX | WS_CHILDWINDOW | WS_VISIBLE, 
        0, 0, 100, FLIP_BUTTON_HEIGHT - 3, 
        ParentWnd, NULL, GetModuleHandle(NULL), NULL);

    //
    // Регистрирум класс окна паттерна.
    //

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = PatternTileProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_HAND);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = PATTERN_TILE_CLASS;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Cannot register Pattern TileWnd Class", "Error", 0);
    }

    //
    // Create pattern label font
    //

    memset(&LogFont, 0, sizeof(LogFont));

    strcpy(LogFont.lfFaceName, "Calibri");
    LogFont.lfWeight = FW_NORMAL;
    LogFont.lfEscapement = 0;

    PatternFont = CreateFontIndirect(&LogFont);

    // Загрузить и распарсить базу данных паттернов.
    f = fopen(dbfile, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        filesize = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char *)malloc(filesize + 1);
        fread(buffer, 1, filesize, f);
        buffer[filesize] = 0;
        fclose(f);
        ParseDatabase(buffer);

        // DEBUG.
        //DumpPatterns();
    }
    else MessageBox(0, "Cannot load patterns database info.", "Error", MB_OK);
}

void PatternResize(int Width, int Height)
{
    int NewWidth = max(100, Width - JpegWindowWidth() - 15);

    MoveWindow(FlipWnd, JpegWindowWidth() + 10, 2, NewWidth, FLIP_BUTTON_HEIGHT - 3, TRUE);

    MoveWindow(PatternWnd, JpegWindowWidth() + 10, FLIP_BUTTON_HEIGHT + 2, NewWidth, max(100, Height - FLIP_BUTTON_HEIGHT - 5), TRUE);

    RearrangePatternTiles();
}

void PatternRedraw(void)
{
    InvalidateRect(PatternWnd, NULL, TRUE);
    UpdateWindow(PatternWnd);
}

PatternItem * PatternGetItem(char * PatternName)
{
    LONG Count;

    for (Count = 0; Count < NumPatterns; Count++)
    {
        if (!_stricmp(Patterns[Count].Name, PatternName)) return &Patterns[Count];
    }

    return NULL;
}

// Уничтожить все ресурсы этого окна, чтобы потом создать их заново.
void PatternDestroy(void)
{
    unsigned Count;

    //
    // Галочка "flip"
    //

    Button_SetCheck(FlipWnd, BST_UNCHECKED);

    //
    // Окна
    //

    for (Count = 0; Count < NumPatterns; Count++)
    {
        DestroyWindow(PatternTiles[Count]);
    }

    //
    // Шаблоны
    //

    if (Patterns)
    {
        free(Patterns);
        Patterns = NULL;
    }
    NumPatterns = 0;

    PatternRedraw();
}

char * GetSavedDatabase(void)
{
    return SavedDatabase;
}