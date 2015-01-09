#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>
#include <setjmp.h>

extern "C"
{
#include "libjpeg\jpeglib.h"
};

/*
    Controls:

    LMB на плоскости Jpeg : создание фильтрующей рамки
    Esc : отмена рамки
    LMB на плоскости Patterns : перетаскивание паттернов
    RMB : скроллинг всех плоскостей

*/

static HWND ParentWnd;
static HWND JpegWnd;

// Source Jpeg
unsigned char *JpegBuffer = NULL;
int JpegBufferSize;
int JpegWidth, JpegHeight;
HBITMAP JpegBitmap;

// Scrolling Control
int ScrollX, ScrollY;
bool ScrollingBegin;
int SavedMouseX, SavedMouseY;
int SavedScrollX, SavedScrollY;

static HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, PBYTE pBits)
{
    HDC MemoryDC = NULL;
    HBITMAP hBitmap = 0;
    if (!uWidth || !uHeight || !uBitsPerPixel)
        return NULL;
    MemoryDC = CreateCompatibleDC(hDC);
    LONG lBmpSize = uWidth * uHeight * (uBitsPerPixel / 8);
    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biBitCount = uBitsPerPixel;
    bmpInfo.bmiHeader.biHeight = uHeight;
    bmpInfo.bmiHeader.biWidth = uWidth;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    // Pointer to access the pixels of bitmap
    unsigned char * pPixels = 0;
    hBitmap = CreateDIBSection(MemoryDC, (BITMAPINFO *)&
        bmpInfo, DIB_RGB_COLORS, (void **)&
        pPixels, NULL, 0);
    DeleteDC(MemoryDC);

    if (hBitmap == NULL)
    {
        MessageBox(NULL, TEXT("\"CreateBitmapFromPixels\" function failed."), TEXT("ERROR!"), MB_ICONERROR);
        return NULL;
    }

    // Правильное заполнение DIB Section.
    // http://stackoverflow.com/questions/21144690/createdibsection-graphical-artifacts-or-how-to-access-bitmap-bits-correctly

    int BytesPerLine = uWidth * 3;
    if (BytesPerLine % 4 != 0)
        BytesPerLine += 4 - BytesPerLine % 4;
    PBYTE Line = NULL;
    PBYTE SrcLine = NULL;
    for (UINT y = uHeight; y > 0; y--)
    {
        Line = (PBYTE)pPixels;
        SrcLine = (PBYTE)pBits + (y-1) * uWidth * 3;
        for (UINT x = 0; x < uWidth; x++)
        {
            Line[0] = SrcLine[2];
            Line[1] = SrcLine[1];
            Line[2] = SrcLine[0];
            Line += 3;
            SrcLine += 3;
        }
        pPixels += BytesPerLine;
    }

    return hBitmap;
}

LRESULT CALLBACK JpegProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HGDIOBJ oldBitmap;
    HDC hdcMem;
    BITMAP bitmap;

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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

        EndPaint(hwnd, &ps);
        break;

    case WM_RBUTTONDOWN:
        ScrollingBegin = true;
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
        }
        break;

    case WM_RBUTTONUP:
        ScrollingBegin = false;
        //InvalidateRect(JpegWnd, NULL, TRUE);
        //UpdateWindow(JpegWnd);
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

    ParentWnd = Parent;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = JpegProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle (NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

struct my_error_mgr {
    struct jpeg_error_mgr pub;    /* "public" fields */

    jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

void JpegAddScanline(unsigned char *buffer, int stride)
{
    //JpegBuffer = (unsigned char *)realloc(JpegBuffer, JpegBufferSize + stride);
    memcpy((void *)(JpegBuffer + JpegBufferSize), buffer, stride);
    JpegBufferSize += stride;
}

// Пример декодирования взят из example.c
void JpegLoadImage(char *filename)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE * infile;
    JSAMPARRAY buffer;        // Output row buffer
    int row_stride;       // physical row width in output buffer
    
    if (JpegBuffer)
    {
        free(JpegBuffer);
        JpegBuffer = NULL;
    }
    JpegBuffer = (unsigned char *)malloc(512 * 1024 * 1024);
    JpegBufferSize = 0;

    if ((infile = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return;
    }

    // Step 1: allocate and initialize JPEG decompression object
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object, close the input file, and return.
        */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return;
    }
    jpeg_create_decompress(&cinfo);

    // Step 2: specify data source (eg, a file)
    jpeg_stdio_src(&cinfo, infile);

    // Step 3: read file parameters with jpeg_read_header() 
    (void)jpeg_read_header(&cinfo, TRUE);

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    // Step 5: Start decompressor
    (void)jpeg_start_decompress(&cinfo);

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    JpegWidth = cinfo.output_width;
    JpegHeight = cinfo.output_height;

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */
        JpegAddScanline(buffer[0], row_stride);
    }

    // Step 7: Finish decompression
    (void)jpeg_finish_decompress(&cinfo);

    // Step 8: Release JPEG decompression object
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);

    if (JpegBitmap)
    {
        DeleteObject(JpegBitmap);
        JpegBitmap = NULL;
    }
    JpegBitmap = CreateBitmapFromPixels(GetWindowDC(JpegWnd), JpegWidth, JpegHeight, 24, JpegBuffer);

    MessageBox(0, "Loaded", "Loaded", MB_OK);

    ScrollX = ScrollY = 0;

    InvalidateRect(JpegWnd, NULL, TRUE);
    UpdateWindow(JpegWnd);
}

// Возвращает true и выбранный регион, если рамка есть
// Возвращает false, если рамки нет
bool JpegGetSelectRegion(LPRECT Region)
{
    return false;
}

// Изменить размер окна Jpeg в соответствии с размерами родительского окна.
void JpegResize(int Width, int Height)
{
    MoveWindow(JpegWnd, 2, 2, max(100, Width - 300), max(100, Height - 2), TRUE);

    // Сбросить регион выделения при изменении размеров окна.
}