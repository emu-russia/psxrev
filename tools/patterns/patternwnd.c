#define _CRT_SECURE_NO_WARNINGS

// Pattern Database pane.

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <Windows.h>
#include <windowsx.h>

#include "jpegwnd.h"
#include "jpegload.h"
#include "statuswnd.h"
#include "patternwnd.h"
#include "profiler.h"
#include "listutils.h"

#define FLIP_BUTTON_HEIGHT  30

static HWND ParentWnd;
static HWND PatternWnd;

extern HWND FlipWnd;
extern HWND MirrorWnd;
extern float WorkspaceLambda, WorkspaceLambdaDelta;
extern char CurrentWorkingDir[MAX_PATH];

static PatternItem * Patterns = NULL;
static int NumPatterns;

static HWND *PatternTiles;

static LIST_ENTRY ViasCollectionHead = { &ViasCollectionHead, &ViasCollectionHead };

#define PATTERN_TILE_CLASS "PatternTile"

static HGDIOBJ PatternFont;
static HGDIOBJ ViasFont;

static HBRUSH GreenBrush;
static HBRUSH RedBrush;
static HBRUSH YellowBrush;
static HBRUSH GrayBrush;

//
// Vias stuff
//

static void ViasClenupCollection ( PViasCollectionEntry Coll )
{
    PViasEntry Vias;

    while ( !IsListEmpty ( &Coll->ViasHead ) )
    {
        Vias = (PViasEntry) Coll->ViasHead.Flink;

        RemoveEntryList ( (PLIST_ENTRY)Vias );

        free (Vias);
    }
}

static void ViasCleanup (void)
{
    PViasCollectionEntry CollEntry;

    while ( !IsListEmpty ( &ViasCollectionHead ) )
    {
        CollEntry = (PViasCollectionEntry)ViasCollectionHead.Flink;
        
        ViasClenupCollection (CollEntry);

        RemoveEntryList ( (PLIST_ENTRY)CollEntry );

        free ( CollEntry );
    }
}

PViasCollectionEntry GetViasCollection ( char * PatternName )
{
    PLIST_ENTRY Entry;
    PViasCollectionEntry CollEntry;

    Entry = ViasCollectionHead.Flink;

    while ( Entry != &ViasCollectionHead )
    {
        CollEntry = (PViasCollectionEntry)Entry;

        if ( !_stricmp ( CollEntry->PatternName, PatternName ) )
            return CollEntry;

        Entry = Entry->Flink;
    }

    return NULL;
}

static PViasCollectionEntry CreateViasCollection ( char * PatternName )
{
    PViasCollectionEntry Coll;

    Coll = (PViasCollectionEntry) malloc ( sizeof(ViasCollectionEntry) );
    if ( Coll == NULL )
        return NULL;

    memset ( Coll, 0, sizeof(ViasCollectionEntry) );

    strcpy ( Coll->PatternName, PatternName );

    InitializeListHead ( &Coll->ViasHead );

    InsertTailList ( &ViasCollectionHead, (PLIST_ENTRY)Coll );

    return Coll;
}

PViasEntry AddVias ( char * PatternName, char * ViasName, float OffsetX, float OffsetY, long Type )
{
    PViasCollectionEntry Coll;
    PViasEntry Vias;

    //
    // Open/create vias collection
    //

    Coll = GetViasCollection (PatternName);
    if ( Coll == NULL )
        Coll = CreateViasCollection ( PatternName );

    if ( Coll == NULL )
        return NULL;

    //
    // Allocate and insert Vias Entry
    //

    Vias = (PViasEntry) malloc ( sizeof(ViasEntry) );
    if ( Vias == NULL )
        return NULL;

    memset ( Vias, 0, sizeof(ViasEntry) );

    strcpy ( Vias->ViasName, ViasName );
    Vias->OffsetX = OffsetX;
    Vias->OffsetY = OffsetY;
    Vias->Type = Type;

    InsertTailList ( &Coll->ViasHead, (PLIST_ENTRY)Vias );

    return Vias;
}

// DEBUG
static void DumpPatterns(void)
{
    int n;

    for (n = 0; n < NumPatterns; n++)
    {
        printf("pattern %i: %s, lambda:%.1f\n", n, Patterns[n].Name, Patterns[n].Lambda );
    }
}

int GetPatternIndexByHwnd(HWND hwnd)
{
    int n;
    for (n = 0; n < NumPatterns; n++)
    {
        if (PatternTiles[n] == hwnd) return n;
    }
    return -1;
}

void DrawPattern ( PatternItem *Item,
                   HDC hdc,
                   LPRECT Rect,
                   BOOL Flipped,
                   BOOL Mirrored,
                   BOOL Box,
                   BOOL Label,
                   BOOL SelectHint,
                   BOOL ViasEnable )
{
    HGDIOBJ oldBitmap;
    HDC hdcMem;
    BITMAP bitmap;
    BLENDFUNCTION Blend;
    HPEN Pen;
    HGDIOBJ OldPen;
    HBRUSH SelectionBrush;
    HGDIOBJ OldBrush;
    HDC hdcSelection;
    BITMAPINFO Bmi;
    HBITMAP SelectionBitmap;
    RECT SelectionRect;
    float LambdaWidth;
    float LambdaHeight;
    int Width;
    int Height;
    int Flags;
    PViasCollectionEntry Coll;
    PLIST_ENTRY Entry;
    PViasEntry Vias;
    #define VIAS_SIZE 8
    int ViasPosX, ViasPosY;
    RECT ViasRect;
    HBRUSH ViasBrush;

    if (Item)
    {
        //
        // Get dimensions
        //

        LambdaWidth = (float)Item->PatternWidth / Item->Lambda;
        LambdaHeight = (float)Item->PatternHeight / Item->Lambda;
        Width = (int)(LambdaWidth * WorkspaceLambda);
        Height = (int)(LambdaHeight * WorkspaceLambda);

        //
        // Draw pattern's images
        //

        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, Item->PatternBitmap);
        GetObject(Item->PatternBitmap, sizeof(bitmap), &bitmap);

        SetStretchBltMode(hdc, HALFTONE);

        Flags = ((int)Mirrored << 1) | (int)Flipped;

        switch (Flags)
        {
            case 0:         // NORM
                StretchBlt(hdc, Rect->left, Rect->top, Rect->right, Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
                break;
            case 1:         // FLIP
                StretchBlt(hdc, Rect->right, Rect->bottom, -Rect->right, -Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
                break;
            case 2:         // MIRROR
                StretchBlt(hdc, Rect->right, Rect->top, -Rect->right, Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
                break;
            case 3:         // MIRROR FLIP
                StretchBlt(hdc, Rect->left, Rect->bottom, Rect->right, -Rect->bottom, hdcMem, 0, 0, Item->PatternWidth, Item->PatternHeight, SRCCOPY);
                break;
        }

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

        //
        // Selection Hint
        //

        if (SelectHint)
        {
            hdcSelection = CreateCompatibleDC(hdc);

            SelectionBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(127,127,127));
            OldBrush = SelectObject(hdcSelection, SelectionBrush);

            memset(&Bmi, 0, sizeof(BITMAPINFO));

            Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            Bmi.bmiHeader.biWidth = Width;
            Bmi.bmiHeader.biHeight = Height;
            Bmi.bmiHeader.biPlanes = 1;
            Bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
            Bmi.bmiHeader.biCompression = BI_RGB;
            Bmi.bmiHeader.biSizeImage = Width * Height * 4;

            SelectionBitmap = CreateDIBSection(hdcSelection, &Bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);
            SelectObject(hdcSelection, SelectionBitmap);

            SetRect(&SelectionRect, 0, 0, Width, Height);

            RoundRect(hdcSelection, 0, 0, Width, Height, 10, 10);

            memset(&Blend, 0, sizeof(BLENDFUNCTION));

            Blend.BlendOp = AC_SRC_OVER;
            Blend.SourceConstantAlpha = 50;
            Blend.AlphaFormat = AC_SRC_ALPHA;

            AlphaBlend(
                hdc, 0, 0, Width, Height,
                hdcSelection, 0, 0, Width, Height, Blend);

            DeleteObject(SelectionBitmap);
            DeleteObject(SelectionBrush);

            DeleteDC(hdcSelection);
        }

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
            // Box
            //

            MoveToEx(hdc, LINE_WIDTH, LINE_WIDTH, NULL);
            LineTo(hdc, Width, 0);
            LineTo(hdc, Width, Height);
            LineTo(hdc, 0, Height);
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
            TextOut(hdc, 0, 0, Item->Name, (int)strlen(Item->Name));
        }

        //
        // Viases
        //

        Coll = GetViasCollection ( Item->Name );

        if ( Coll && ViasEnable )
        {
            Entry = Coll->ViasHead.Flink;

            while ( Entry != &Coll->ViasHead )
            {
                Vias = (PViasEntry) Entry;

                ViasPosX = (int)(Vias->OffsetX * WorkspaceLambda);
                ViasPosY = (int)(Vias->OffsetY * WorkspaceLambda);

                switch (Flags)
                {
                    case 0:
                    default:
                        ViasPosX -= VIAS_SIZE / 2;
                        ViasPosY -= VIAS_SIZE / 2;
                        break;
                    case 1:
                        ViasPosX = Width - ViasPosX - VIAS_SIZE / 2;
                        ViasPosY = Height - ViasPosY - VIAS_SIZE / 2;
                        break;
                    case 2:
                        ViasPosX = Width - ViasPosX - VIAS_SIZE / 2;
                        break;
                    case 3:
                        ViasPosY = Height - ViasPosY - VIAS_SIZE / 2;
                        break;
                }

                //
                // Entity
                //

                switch ( Vias->Type )
                {
                    case ViasInput:
                        ViasBrush = GreenBrush;
                        break;
                    case ViasOutput:
                        ViasBrush = RedBrush;
                        break;
                    case ViasInout:
                        ViasBrush = YellowBrush;
                        break;
                    default:
                        ViasBrush = GrayBrush;
                        break;
                }

                SetRect ( &ViasRect,
                          ViasPosX, ViasPosY,
                          ViasPosX + VIAS_SIZE, ViasPosY + VIAS_SIZE);

                FillRect ( hdc, &ViasRect, ViasBrush );

                //
                // Label
                //

                SelectObject(hdc, ViasFont);
                TextOut(hdc, ViasPosX + VIAS_SIZE, ViasPosY,
                             Vias->ViasName, (int)strlen(Vias->ViasName) );

                Entry = Entry->Flink;
            }
        }
    }
    else
    {
        SelectObject(hdc, PatternFont);
        TextOut(hdc, 0, 0, "UNKNOWN", (int)strlen("UNKNOWN"));
    }
}

LRESULT CALLBACK PatternTileProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT Rect;
    int PatternIndex;
    PatternItem *Item;
    BOOL Flipped;
    BOOL Mirrored;

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
            Mirrored = Button_GetCheck(MirrorWnd) == BST_CHECKED;
            if (!Item->Hidden) DrawPattern(Item, hdc, &Rect, Flipped, Mirrored, FALSE, FALSE, FALSE, FALSE);
        }

        EndPaint(hwnd, &ps);
        break;

    case WM_ERASEBKGND:
        return TRUE;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

BOOL CheckHidden(int PatternIndex)
{
    PatternItem * Item = &Patterns[PatternIndex];
    RECT Region;
    BOOL RegionSelected = JpegGetSelectRegion(&Region);
    BOOL Fit;

    float RegionWidth, RegionHeight;
    float LambdaWidth, LambdaHeight;
    float PLambdaWidth, PLambdaHeight;

    if (RegionSelected)
    {
        //
        // Calculate select box width and height in lambdas
        //

        RegionWidth = (float)abs(Region.right - Region.left);
        RegionHeight = (float)abs(Region.bottom - Region.top);
        LambdaWidth = RegionWidth / WorkspaceLambda;
        LambdaHeight = RegionHeight / WorkspaceLambda;

        //
        // Calculate pattern width and height in lambdas
        //

        PLambdaWidth = (float)Item->PatternWidth / Item->Lambda;
        PLambdaHeight = (float)Item->PatternHeight / Item->Lambda;

        //
        // Match dimensions according to lambda delta
        //

        Fit = FALSE;
        if (PLambdaWidth >= (LambdaWidth - 2 * WorkspaceLambdaDelta) && 
			PLambdaWidth < (LambdaWidth + 2 * WorkspaceLambdaDelta))
        {
            if (PLambdaHeight >= (LambdaHeight - 2 * WorkspaceLambdaDelta) && 
				PLambdaHeight < (LambdaHeight + 2 * WorkspaceLambdaDelta)) Fit = TRUE;
        }

        //
        // Special garbage cell is always shown
        //

        if (!_stricmp(Item->Name, "GARBAGE")) Fit = TRUE;

        return !Fit;
    }
    else return FALSE;
}

// Line up child windows in thumbnail-like order
// Called after:
//   - Changing parent window size
//   - Flip checkbox was pressed
//   - Adding new pattern from library
void RearrangePatternTiles(void)
{
    int n;
    RECT Rect;
    char Text[0x100];
    int Border;
    int Width;
    int TileWidth;
    int TileHeight;
    float Aspect;
    int HorPos, VerPos;
    int ColCount = 0, RowCount = 0;
    int MaxHeight = 0;
    int ShownPatterns = 0;

    GetClientRect (PatternWnd, &Rect);

    Border = 2;
    Width = Rect.right;
    TileWidth = Width / 3 - (Border << 1);
    VerPos = Border;

    for (n = 0; n < NumPatterns; n++)
    {
        if (ColCount == 0) HorPos = Border;

        Aspect = (float)Patterns[n].PatternWidth / (float)TileWidth;
        TileHeight = (int) floor ( (float)Patterns[n].PatternHeight / Aspect );

        if (CheckHidden(n))
        {
            Patterns[n].Hidden = TRUE;
            MoveWindow(PatternTiles[n], 0, 0, 0, 0, TRUE);
            continue;
        }
        else
        {
            Patterns[n].Hidden = FALSE;
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
void AddNewPattern(char *name, char *jpeg_path, float lambda, int pcount, int ncount, long type)
{
    PatternItem Item;
    int DecodeResult;
    char Text[1024];
    char FullPath[MAX_PATH];

    memset(&Item, 0, sizeof(PatternItem));

    strncpy(Item.Name, name, sizeof(Item.Name) - 1);
    Item.Lambda = lambda;
    Item.pcount = pcount;
    Item.ncount = ncount;
    Item.Hidden = FALSE;
    Item.Type = type;

    sprintf(FullPath, "%s\\%s", CurrentWorkingDir, jpeg_path);

    DecodeResult = JpegLoad(
        FullPath,
        PatternAddScanline,
        &Item.PatternRawImage,
        &Item.PatternBufferSize,
        &Item.PatternWidth,
        &Item.PatternHeight,
        &Item);

    if (DecodeResult < 0)
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
    //sprintf(buffer, "Add Pattern: name:%s, lambda:%f, pcount:%i, ncount:%i, path:[%s]\n", name, lambda, pcount, ncount, jpeg_path);
    //MessageBox(0, buffer, "New Pattern", 0);
}

// Trim leading and trailing whitespaces and quotes in given string.
char *TrimString(char *str)
{
    int length, n;
    while (*str == ' ' || *str == '\"') str++;
    length = (int)strlen(str);
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
    int spacesAtStart = (int)strspn(str, WHITESPACE);
    char *result = str + spacesAtStart;
    int lengthOfNonSpace = (int)strcspn(result, WHITESPACE);
    result[lengthOfNonSpace] = 0;
    return result;
}

void ParseLine(char *line)
{
    char c;
    char command[32];
    char name[128];
    char typeStr[128];      // vias_name
    char *typeStrPtr;
    long type;
    char lambda[32];         // offset_x
    char pcount[32];        // offset_y
    char ncount[32];        // type
    char path[256];         // reserved

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

    sscanf(line, "%s %[^','],%[^','],%[^','],%[^','],%[^','],%s", command, name, typeStr, lambda, pcount, ncount, path);

    if (!_stricmp(command, "pattern"))
    {
        typeStrPtr = typeStr;
        while ( *typeStrPtr <= ' ' && *typeStrPtr )
            typeStrPtr++;

        if ( !_stricmp ( typeStrPtr, "CellNot" ) )
            type = CellNot;
        else if ( !_stricmp ( typeStrPtr, "CellBuffer" ) )
            type = CellBuffer;
        else if ( !_stricmp ( typeStrPtr, "CellMux" ) )
            type = CellMux;
        else if ( !_stricmp ( typeStrPtr, "CellLogic" ) )
            type = CellLogic;
        else if ( !_stricmp ( typeStrPtr, "CellAdder" ) )
            type = CellAdder;
        else if ( !_stricmp ( typeStrPtr, "CellBusSupp" ) )
            type = CellBusSupp;
        else if ( !_stricmp ( typeStrPtr, "CellFlipFlop" ) )
            type = CellFlipFlop;
        else if ( !_stricmp ( typeStrPtr, "CellLatch" ) )
            type = CellLatch;
        else if ( !_stricmp ( typeStrPtr, "CellOther" ) )
            type = CellOther;
        else
        {
            MessageBox ( NULL, typeStr, "Unknown pattern type!", MB_ICONASTERISK );
            return;
        }

        AddNewPattern(TrimString(name), TrimString(path), (float)atof(lambda), atoi(pcount), atoi(ncount), type);
    }
    else if (!_stricmp(command, "vias") )
    {
        typeStrPtr = ncount;
        while ( *typeStrPtr <= ' ' && *typeStrPtr )
            typeStrPtr++;

        if ( !_stricmp ( typeStrPtr, "ViasInput" ) )
            type = ViasInput;
        else if ( !_stricmp ( typeStrPtr, "ViasOutput" ) )
            type = ViasOutput;
        else if ( !_stricmp ( typeStrPtr, "ViasInout" ) )
            type = ViasInout;
        else
        {
            MessageBox ( NULL, typeStr, "Unknown vias type!", MB_ICONASTERISK );
            return;
        }

        AddVias ( TrimString(name), 
                  TrimString(typeStr),
                  (float)atof(lambda),
                  (float)atof(pcount),
                  type );
    }

#if defined(_DEBUG)
    printf( "Case: command:%s, name:%s, lambda:%s, pcount:%s, ncount:%s, path:%s\n",
             command, name, lambda, pcount, ncount, path);
#endif
}

void ParseDatabase(char *text)
{
    char Text[0x100];
    char linebuffer[0x10000], *ptr = text;
    char *lineptr = linebuffer, c = *ptr;

    // Line by line.
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
    LOGFONT LogFont;

    ParentWnd = Parent;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
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
    // Create "flip" checkbox
    //

    FlipWnd = CreateWindow(
        "BUTTON",
        "flip",
        BS_CHECKBOX | WS_CHILDWINDOW | WS_VISIBLE,
        0, 0, 100, FLIP_BUTTON_HEIGHT - 3,
        ParentWnd, NULL, GetModuleHandle(NULL), NULL);

    //
    // Create "mirror" checkbox
    //

    MirrorWnd = CreateWindow(
        "BUTTON",
        "mirror",
        BS_CHECKBOX | WS_CHILDWINDOW | WS_VISIBLE,
        0, 0, 100, FLIP_BUTTON_HEIGHT - 3,
        ParentWnd, NULL, GetModuleHandle(NULL), NULL);

    //
    // Register pattern window class
    //

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
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

    if ( PatternFont == NULL )
    {
        memset(&LogFont, 0, sizeof(LogFont));

        strcpy(LogFont.lfFaceName, "Calibri");
        LogFont.lfWeight = FW_NORMAL;
        LogFont.lfEscapement = 0;

        PatternFont = CreateFontIndirect(&LogFont);
    }

    //
    // Create vias label font
    //

    if ( ViasFont == NULL )
    {
        memset(&LogFont, 0, sizeof(LogFont));

        strcpy(LogFont.lfFaceName, "Calibri");
        LogFont.lfWeight = FW_NORMAL;
        LogFont.lfHeight = 10;
        LogFont.lfEscapement = 0;

        ViasFont = CreateFontIndirect(&LogFont);
    }

    //
    // Brushes
    //

    if ( GreenBrush == NULL )
        GreenBrush = CreateSolidBrush ( RGB(0,255,0) );

    if ( RedBrush == NULL )
        RedBrush = CreateSolidBrush ( RGB(255,0,0) );

    if ( YellowBrush == NULL )
        YellowBrush = CreateSolidBrush ( RGB(255,255,0) );

    if ( GrayBrush == NULL )
        GrayBrush = CreateSolidBrush ( RGB(0x7f,0x7f,0x7f) );

    // Load and parse pattern database.
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

    MoveWindow(FlipWnd, JpegWindowWidth() + 10, 2, NewWidth / 2, FLIP_BUTTON_HEIGHT - 3, TRUE);
    MoveWindow(MirrorWnd, JpegWindowWidth() + NewWidth / 2 + 10, 2, NewWidth / 2, FLIP_BUTTON_HEIGHT - 3, TRUE);

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

// Destroy all system resource, to create it again.
void PatternDestroy(void)
{
    int Count;

    //
    // "flip" checkbox
    //

    Button_SetCheck(FlipWnd, BST_UNCHECKED);
    Button_SetCheck(MirrorWnd, BST_UNCHECKED);

    //
    // Windows
    //

    for (Count = 0; Count < NumPatterns; Count++)
    {
        DestroyWindow(PatternTiles[Count]);
    }

    //
    // Templates
    //

    if (Patterns)
    {
        free(Patterns);
        Patterns = NULL;
    }
    NumPatterns = 0;

    //
    // Vias
    //

    ViasCleanup ();

    PatternRedraw();
}
