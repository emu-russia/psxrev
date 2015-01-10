#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>

#include "jpegload.h"
#include "patternwnd.h"
#include "statuswnd.h"

/*
Controls:

LMB на плоскости Jpeg : создание фильтрующей рамки
Esc : отмена рамки
Home : ScrollX = ScrollY = 0
LMB на плоскости Patterns : перетаскивание паттернов
RMB : скроллинг всех плоскостей

*/

extern float WorkspaceLamda, WorkspaceLamdaDelta;

static HWND ParentWnd;
static HWND JpegWnd;

// Source Jpeg
static unsigned char *JpegBuffer = NULL;
static int JpegBufferSize;
static int JpegWidth, JpegHeight;
static HBITMAP JpegBitmap;

// Scrolling / Region Selection Control
static int ScrollX, ScrollY;
static bool ScrollingBegin;
static int SavedMouseX, SavedMouseY;
static int SavedScrollX, SavedScrollY;
static bool SelectionBegin, RegionSelected;
static int SelectionStartX, SelectionStartY;
static int SelectionEndX, SelectionEndY;

static void UpdateSelectionStatus(void)
{
    char Text[0x100];
    int Width, Height;
    float LamdaWidth, LamdaHeight;

    Width = abs(SelectionEndX - SelectionStartX);
    Height = abs(SelectionEndY - SelectionStartY);

    if (RegionSelected && Width > 10 && Height > 10)
    {
        LamdaWidth = (float)Width / WorkspaceLamda;
        LamdaHeight = (float)Height / WorkspaceLamda;
        sprintf(Text, "Selected: %i / %ipx, %.1f / %.1fl", Width, Height, LamdaWidth, LamdaHeight);
        SetStatusText(STATUS_SELECTED, Text);
    }
    else SetStatusText(STATUS_SELECTED, "Selected: ---");
}

LRESULT CALLBACK JpegProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HGDIOBJ oldBitmap;
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldColor;
    RECT Rect;
    char Text[0x100];

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        if (JpegBitmap)
        {
            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, JpegBitmap);

            GetObject(JpegBitmap, sizeof(bitmap), &bitmap);
            BitBlt(hdc, ScrollX, ScrollY, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
        }

        // Рамка выделения.
        if (RegionSelected)
        {
            oldColor = SelectObject(hdc, GetStockObject(DC_PEN));
            SetDCPenColor(hdc, RGB(0xaa, 0xaa, 0xaa));
            MoveToEx(hdc, SelectionStartX, SelectionStartY, NULL);
            LineTo(hdc, SelectionEndX, SelectionStartY);
            LineTo(hdc, SelectionEndX, SelectionEndY);
            LineTo(hdc, SelectionStartX, SelectionEndY);
            LineTo(hdc, SelectionStartX, SelectionStartY);
            SelectObject(hdc, oldColor);
        }

        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        RegionSelected = false;
        SelectionBegin = true;
        SelectionStartX = LOWORD(lParam);
        SelectionStartY = HIWORD(lParam);
        InvalidateRect(JpegWnd, NULL, TRUE);
        UpdateWindow(JpegWnd);
        break;

    case WM_RBUTTONDOWN:
        ScrollingBegin = true;
        RegionSelected = false;
        SavedMouseX = LOWORD(lParam);
        SavedMouseY = HIWORD(lParam);
        SavedScrollX = ScrollX;
        SavedScrollY = ScrollY;
        break;

    case WM_MOUSEMOVE:
        if (ScrollingBegin)
        {
            ScrollX = SavedScrollX + LOWORD(lParam) - SavedMouseX;
            ScrollY = SavedScrollY + HIWORD(lParam) - SavedMouseY;
            InvalidateRect(JpegWnd, NULL, FALSE);
            UpdateWindow(JpegWnd);

            sprintf(Text, "Scroll : %i / %ipx", ScrollX, ScrollY);
            SetStatusText(STATUS_SCROLL, Text);
        }
        if (SelectionBegin)
        {
            SelectionEndX = LOWORD(lParam);
            SelectionEndY = HIWORD(lParam);
            RegionSelected = true;
            Rect.left = SelectionStartX;
            Rect.top = SelectionStartY;
            Rect.right = SelectionEndX + 1;
            Rect.bottom = SelectionEndY + 1;
            InvalidateRect(JpegWnd, &Rect, TRUE);
            UpdateWindow(JpegWnd);
            RearrangePatternTiles();
            PatternRedraw();
            UpdateSelectionStatus();
        }
        break;

    case WM_RBUTTONUP:
        ScrollingBegin = false;
        //InvalidateRect(JpegWnd, NULL, TRUE);
        //UpdateWindow(JpegWnd);

        sprintf(Text, "Scroll : %i / %ipx", ScrollX, ScrollY );
        SetStatusText(STATUS_SCROLL, Text);
        break;

    case WM_LBUTTONUP:
        SelectionBegin = false;
        InvalidateRect(JpegWnd, NULL, TRUE);
        UpdateWindow(JpegWnd);
        RearrangePatternTiles();
        PatternRedraw();
        UpdateSelectionStatus();
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void JpegInit(HWND Parent)
{
    WNDCLASSEX wc;

    ScrollX = ScrollY = 0;
    ScrollingBegin = false;
    SelectionBegin = RegionSelected = false;

    ParentWnd = Parent;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = JpegProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "JpegWnd";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Cannot register JpgWnd Class", "Error", 0);
        return;
    }

    JpegWnd = CreateWindowEx(
        0,
        "JpegWnd",
        "JpegWndPopup",
        WS_OVERLAPPED | WS_CHILDWINDOW,
        2,
        2,
        100,
        100,
        ParentWnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(JpegWnd, SW_NORMAL);
    UpdateWindow(JpegWnd);
}

static void JpegAddScanline(unsigned char *buffer, int stride, void *Param)
{
    memcpy((void *)(JpegBuffer + JpegBufferSize), buffer, stride);
    JpegBufferSize += stride;
}

// Пример декодирования взят из example.c
void JpegLoadImage(char *filename)
{
    char Text[1024];

    JpegLoad(
        filename,
        JpegAddScanline,
        &JpegBuffer,
        &JpegBufferSize,
        &JpegWidth,
        &JpegHeight,
        NULL );

    if (JpegBitmap)
    {
        DeleteObject(JpegBitmap);
        JpegBitmap = NULL;
    }
    if (JpegBuffer)
        JpegBitmap = CreateBitmapFromPixels(GetWindowDC(JpegWnd), JpegWidth, JpegHeight, 24, JpegBuffer);

    MessageBox(0, "Loaded", "Loaded", MB_OK);

    sprintf(Text, "Source Image : %s", filename);
    SetStatusText(STATUS_SOURCE_IMAGE, Text);

    ScrollX = ScrollY = 0;

    InvalidateRect(JpegWnd, NULL, TRUE);
    UpdateWindow(JpegWnd);
}

// Возвращает true и выбранный регион, если рамка есть
// Возвращает false, если рамки нет
bool JpegGetSelectRegion(LPRECT Region)
{
    int Width, Height;

    if (RegionSelected)
    {
        Width = abs(SelectionEndX - SelectionStartX);
        Height = abs(SelectionEndY - SelectionStartY);

        if (Width * Height < 100) return false;

        Region->left = SelectionStartX;
        Region->top = SelectionStartY;
        Region->right = SelectionEndX;
        Region->bottom = SelectionEndY;

        return true;
    }
    else return false;
}

// Изменить размер окна Jpeg в соответствии с размерами родительского окна.
void JpegResize(int Width, int Height)
{
    MoveWindow(JpegWnd, 2, 2, max(100, Width - 300), max(100, Height - 5), TRUE);

    // Сбросить регион выделения при изменении размеров окна.
    RegionSelected = false;
    SetStatusText(STATUS_SELECTED, "Selected: ---");
}

// Вернуть ширину окна.
int JpegWindowWidth(void)
{
    RECT Rect;
    GetClientRect(JpegWnd, &Rect);
    return Rect.right;
}