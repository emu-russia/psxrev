#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <windowsx.h>
#include "resource.h"

#include <stdio.h>
#include <math.h>

#include "jpegload.h"
#include "patternwnd.h"
#include "statuswnd.h"
#include "jpegwnd.h"
#include "profiler.h"

#ifdef USEGL
#include <GL/gl.h>
#pragma comment(lib, "opengl32.lib")
#endif

/*
Controls:

LMB over source image : Selection box
Esc : cancel selection
Home : ScrollX = ScrollY = 0
LMB over patterns : drag & drop patterns
Double click on pattern : flip it
RMB over empty space : Scrolling

*/

extern HWND FlipWnd;
extern float WorkspaceLamda, WorkspaceLamdaDelta;

LRESULT CALLBACK JpegProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static HWND ParentWnd;
static HWND JpegWnd;
static HDC JpegOffscreenDC;

// Source Jpeg
static unsigned char *JpegBuffer = NULL;
static long JpegBufferSize;
static long JpegWidth, JpegHeight;
static HBITMAP JpegBitmap;

// Scrolling / Region Selection Control
static int ScrollX, ScrollY;
static bool ScrollingBegin;
static int SavedMouseX, SavedMouseY;
static int SavedScrollX, SavedScrollY;
static bool SelectionBegin, RegionSelected;
static int SelectionStartX, SelectionStartY;
static int SelectionEndX, SelectionEndY;
static BOOL DragOccureInPattern;

// Added Patterns Layer.

static PatternEntry * PatternLayer;
static int NumPatterns;
static PatternEntry * SelectedPattern;
static bool DraggingPattern;

HBITMAP RemoveBitmap;
#define REMOVE_BITMAP_WIDTH 12

#define PATTERN_ENTRY_CLASS "PatternEntry"

static char * SavedImageName = NULL;

#ifdef USEGL

//
// OpenGL stuff
//

static HGLRC hGLRC;

typedef struct _MESH_ENTRY
{
    int         Vertex0[2];
    int         Vertex1[2];
    int         Vertex2[2];
    int         Vertex3[2];
    GLuint      TextureID;
    PUCHAR      BitmapBlock;
} MESH_ENTRY, *PMESH_ENTRY;

// 0 : Big mesh
// 1 : Right-side smaller mesh
// 2 : Bootom-side smaller mesh

typedef struct _MESH_BUCKET
{
    PMESH_ENTRY Mesh[3];
    int MeshWidth[3];
    int MeshHeight[3];
} MESH_BUCKET, *PMESH_BUCKET;

MESH_BUCKET JpegMesh;

#define JPEG_MESH_LOG2   7
#define JPEG_SMALLER_MESH_LOG2   5

#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static void SetupPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  /* size */
        1,                              /* version */
        PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,               /* support double-buffering */
        PFD_TYPE_RGBA,                  /* color type */
        16,                             /* prefered color depth */
        0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
        0,                              /* no alpha buffer */
        0,                              /* alpha bits (ignored) */
        0,                              /* no accumulation buffer */
        0, 0, 0, 0,                     /* accum bits (ignored) */
        16,                             /* depth buffer */
        0,                              /* no stencil buffer */
        0,                              /* no auxiliary buffers */
        PFD_MAIN_PLANE,                 /* main layer */
        0,                              /* reserved */
        0, 0, 0,                        /* no layer, visible, damage masks */
    };
    int pixelFormat;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    if (pixelFormat == 0) {
        MessageBox(WindowFromDC(hDC), "ChoosePixelFormat failed.", "Error",
            MB_ICONERROR | MB_OK);
        exit(1);
    }

    if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) {
        MessageBox(WindowFromDC(hDC), "SetPixelFormat failed.", "Error",
            MB_ICONERROR | MB_OK);
        exit(1);
    }
}

static PMESH_ENTRY JpegGenerateMesh(int Width, int Height, unsigned char *BitmapData, int *MeshWidth, int *MeshHeight, int MeshLog2, long Stride)
{
    PMESH_ENTRY Mesh;
    PMESH_ENTRY Entry;
    int Rows;
    int Columns;
    int RowCounter;
    int ColumnCounter;
    int ImageX, ImageY, RowStride;
    int ImageRow;
    PUCHAR ImagePointer;
    int BlockWidth;

#define MESH_LOG2  MeshLog2

    Rows = Height >> MESH_LOG2;
    Columns = Width >> MESH_LOG2;

    *MeshWidth = Rows;
    *MeshHeight = Columns;

    Mesh = (PMESH_ENTRY)malloc(Rows * Columns * sizeof(MESH_ENTRY));
    if (!Mesh) return NULL;

    memset(Mesh, 0, Rows * Columns * sizeof(MESH_ENTRY));

    BlockWidth = 1 << MESH_LOG2;

    for (RowCounter = 0; RowCounter < Rows; RowCounter++)
    {
        for (ColumnCounter = 0; ColumnCounter < Columns; ColumnCounter++)
        {
            Entry = &Mesh[RowCounter * Columns + ColumnCounter];

            Entry->Vertex0[0] = ColumnCounter << MESH_LOG2;
            Entry->Vertex0[1] = RowCounter << MESH_LOG2;
            Entry->Vertex1[0] = Entry->Vertex0[0] + BlockWidth;
            Entry->Vertex1[1] = Entry->Vertex0[1];
            Entry->Vertex2[0] = Entry->Vertex0[0];
            Entry->Vertex2[1] = Entry->Vertex0[1] + BlockWidth;
            Entry->Vertex3[0] = Entry->Vertex0[0] + BlockWidth;
            Entry->Vertex3[1] = Entry->Vertex0[1] + BlockWidth;

            glGenTextures(1, &Entry->TextureID);

            glBindTexture(GL_TEXTURE_2D, Entry->TextureID);

            //
            // Copy bitmap sub-region
            //

            Entry->BitmapBlock = (PUCHAR)malloc(BlockWidth * BlockWidth * 3);
            if (!Entry->BitmapBlock)
            {
                free(Mesh);
                return NULL;
            }

            ImageX = ColumnCounter * BlockWidth;
            ImageY = RowCounter * BlockWidth;
            RowStride = Width * 3 + Stride;
            ImagePointer = &BitmapData[(ImageY * Width + ImageX) * 3 + Stride];

            for (ImageRow = 0; ImageRow < BlockWidth; ImageRow++, ImagePointer += RowStride)
            {
                memcpy(&Entry->BitmapBlock[ImageRow * BlockWidth * 3],
                    ImagePointer,
                    BlockWidth * 3);
            }

            //
            // Set texture Data
            //

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, (1 << MESH_LOG2), (1 << MESH_LOG2), 0, GL_RGB, GL_UNSIGNED_BYTE, Entry->BitmapBlock);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }

    return Mesh;
}

static void JpegDrawMesh(PMESH_ENTRY Mesh, int MeshWidth, int MeshHeight)
{
    PMESH_ENTRY Entry;
    int RowCounter;
    int ColumnCounter;

    if (Mesh == NULL) return;

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_SRC_COLOR);

    for (RowCounter = 0; RowCounter < MeshHeight; RowCounter++)
    {
        for (ColumnCounter = 0; ColumnCounter < MeshWidth; ColumnCounter++)
        {
            Entry = &Mesh[RowCounter * MeshWidth + ColumnCounter];

            //
            // Skip invisible meshes
            //

            if ((Entry->Vertex0[0] + ScrollX) > JpegWindowWidth()) continue;
            if ((Entry->Vertex1[0] + ScrollX) < 0) continue;
            if ((Entry->Vertex0[1] + ScrollY) > JpegWindowHeight()) continue;
            if ((Entry->Vertex2[1] + ScrollY) < 0) continue;

            //
            // Draw mesh
            //

            glBindTexture(GL_TEXTURE_2D, Entry->TextureID);

            glBegin(GL_QUADS);
            glNormal3d(1, 0, 0);
            glTexCoord2f(0, 0);
            glVertex2i(Entry->Vertex0[0] + ScrollX, Entry->Vertex0[1] + ScrollY);
            glTexCoord2f(1, 0);
            glVertex2i(Entry->Vertex1[0] + ScrollX, Entry->Vertex1[1] + ScrollY);
            glTexCoord2f(1, 1);
            glVertex2i(Entry->Vertex3[0] + ScrollX, Entry->Vertex3[1] + ScrollY);
            glTexCoord2f(0, 1);
            glVertex2i(Entry->Vertex2[0] + ScrollX, Entry->Vertex2[1] + ScrollY);
            glEnd();

        }
    }

    glDisable(GL_TEXTURE_2D);
}

static void JpegMeshRelease(PMESH_BUCKET Bucket)
{
    int MeshCount;
    int EntryCount;
    int Count;

    for (MeshCount = 0; MeshCount < 3; MeshCount++)
    {
        EntryCount = Bucket->MeshWidth[MeshCount] * Bucket->MeshHeight[MeshCount];

        for (Count = 0; Count < EntryCount; Count++)
        {
            if (Bucket->Mesh[MeshCount][Count].BitmapBlock)
            {
                glDeleteTextures(1, &Bucket->Mesh[MeshCount][Count].TextureID);

                free(Bucket->Mesh[MeshCount][Count].BitmapBlock);
            }
        }

        if (Bucket->Mesh[MeshCount])
        {
            free(Bucket->Mesh[MeshCount]);
            Bucket->Mesh[MeshCount] = NULL;
            Bucket->MeshWidth[MeshCount] = 0;
            Bucket->MeshHeight[MeshCount] = 0;
        }
    }
}

static void MakeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
        }
    }
}

static void GL_init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    MakeCheckImage();
}

static void GL_redraw(HDC hDC)
{
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

    //
    // Draw Jpeg mesh
    //

    if (JpegBuffer)
    {
        JpegDrawMesh(JpegMesh.Mesh[0], JpegMesh.MeshWidth[0], JpegMesh.MeshHeight[0]);
    }

    //
    // Draw added patterns
    //

    /*
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_SRC_COLOR);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glNormal3d(1, 0, 0);
    glTexCoord2f(0, 1);
    glVertex2i(0 + ScrollX, 0 + ScrollY);
    glTexCoord2f(0, 0);
    glVertex2i(0 + ScrollX, 100 + ScrollY);
    glTexCoord2f(1, 0);
    glVertex2i(100 + ScrollX, 100 + ScrollY);
    glTexCoord2f(1, 1);
    glVertex2i(100 + ScrollX, 0 + ScrollY);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    */

    //
    // Draw selection box
    //

    if (RegionSelected)
    {
        glLineWidth(1.5);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2i(SelectionStartX, SelectionStartY);
        glVertex2i(SelectionEndX, SelectionStartY);
        glVertex2i(SelectionEndX, SelectionStartY);
        glVertex2i(SelectionEndX, SelectionEndY);
        glVertex2i(SelectionEndX, SelectionEndY);
        glVertex2i(SelectionStartX, SelectionEndY);
        glVertex2i(SelectionStartX, SelectionEndY);
        glVertex2i(SelectionStartX, SelectionStartY);
        glEnd();
    }

    /*
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, 1.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();
    */

    SwapBuffers(hDC);
}

static void GL_resize(int winWidth, int winHeight)
{
    /* set viewport to cover the window */
    glViewport(0, 0, winWidth, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, winWidth, winHeight, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

#endif // USEGL

static int GetPatternEntryIndexByHwnd(HWND Hwnd)
{
    int n;
    for (n = 0; n < NumPatterns; n++)
    {
        if (PatternLayer[n].Hwnd == Hwnd) return n;
    }
    return -1;
}

static void SaveEntryPositions(void)
{
    int n;

    for (n = 0; n < NumPatterns; n++)
    {
        PatternLayer[n].SavedPosX = PatternLayer[n].PosX;
        PatternLayer[n].SavedPosY = PatternLayer[n].PosY;
    }
}

static void UpdateEntryPositions(int OffsetX, int OffsetY, BOOLEAN Update)
{
    int n;
    int OldPosX;
    int OldPosY;

    PERF_START("UpdateEntryPositions");

    for (n = 0; n < NumPatterns; n++)
    {
        OldPosX = PatternLayer[n].PosX;
        OldPosY = PatternLayer[n].PosY;
        PatternLayer[n].PosX = PatternLayer[n].SavedPosX + OffsetX;
        PatternLayer[n].PosY = PatternLayer[n].SavedPosY + OffsetY;

        if (OldPosX != PatternLayer[n].PosX || OldPosY != PatternLayer[n].PosY)
        {

            //
            // WDM is weak on windows movement.
            //

#if 1
#ifndef USEGL
            MoveWindow(
                PatternLayer[n].Hwnd,
                PatternLayer[n].PosX, PatternLayer[n].PosY,
                PatternLayer[n].Width, PatternLayer[n].Height, Update );
#endif  // USEGL
#endif
        }
    }

    //Entry->PosX = Entry->SavedPosX + OffsetX;
    //Entry->PosY = Entry->SavedPosY + OffsetY;
    //MoveWindow(Entry->Hwnd, Entry->PosX, Entry->PosY, Entry->Width, Entry->Height, Update);

    PERF_STOP("UpdateEntryPositions");
}

// Delete cell from patterns layer.
//  - Delete pattern window
//  - Rearrange patterns array without counting removed cell
static void RemovePatternEntry(int EntryIndex)
{
    PatternEntry * Entry = &PatternLayer[EntryIndex];
    PatternEntry * TempList;
    int Count, Index;
    char Text[0x100];

#ifndef USEGL
    DestroyWindow(Entry->Hwnd);
#endif

    TempList = (PatternEntry *)malloc(sizeof(PatternEntry)* (NumPatterns - 1));

    for (Count = 0, Index = 0; Count < NumPatterns; Count++)
    {
        if (Count != EntryIndex) TempList[Index++] = PatternLayer[Count];
    }

    free(PatternLayer);
    PatternLayer = TempList;
    NumPatterns--;

    //
    // Update status line.
    //
    sprintf(Text, "Patterns Added : %i", NumPatterns);
    SetStatusText(STATUS_ADDED, Text);
}

#ifndef USEGL

static LRESULT CALLBACK PatternEntryProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdcMem;
    HGDIOBJ oldBitmap;
    BITMAP bitmap;
    RECT Rect;
    int EntryIndex;
    PatternEntry * Entry;
    PatternItem * Item;
    int CursorX, CursorY;
    char Text[0x1000];
    PatternEntry *Selected;

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        break;

    //
    // Drag window
    //

    case WM_MOVE:
        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            Entry->PosX = (int)(short)LOWORD(lParam);
            Entry->PosY = (int)(short)HIWORD(lParam);

            //
            // Update Plane coords
            //

            Entry->PlaneX = Entry->PosX - ScrollX;
            Entry->PlaneY = Entry->PosY - ScrollY;

            //
            // Update status line by selection
            //

            Selected = JpegGetSelectedPattern();

            if (Selected)
            {
                sprintf(
                    Text, "Selected: %s, Plane: %i,%i, Pos: %i:%i",
                    Selected->PatternName, Selected->PlaneX, Selected->PlaneY, Selected->PosX, Selected->PosY);
                SetStatusText(STATUS_SELECTED, Text);
            }
        }
        break;

    case WM_MOUSEMOVE:
        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (wParam == MK_LBUTTON && EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            CursorX = (int)(short)LOWORD(lParam);
            CursorY = (int)(short)LOWORD(lParam);
            if (!(CursorY < REMOVE_BITMAP_WIDTH && CursorX >= (Entry->Width - REMOVE_BITMAP_WIDTH)))
            {
                SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
            }
        }
        return FALSE;

    //
    // Pattern Selection
    //

    case WM_LBUTTONDOWN:
        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            JpegSelectPattern(Entry);
            DraggingPattern = TRUE;
            return TRUE;
        }
        return FALSE;

    //
    // Pattern remove button press handler.
    //

    case WM_LBUTTONUP:
        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            CursorX = LOWORD(lParam);
            CursorY = HIWORD(lParam);

            if (CursorY < REMOVE_BITMAP_WIDTH && CursorX >= (Entry->Width - REMOVE_BITMAP_WIDTH))
            {
                if (MessageBox(NULL, "Are you sure?", "User confirm", MB_ICONQUESTION | MB_YESNO) == IDYES)
                {
                    RemovePatternEntry(EntryIndex);
                }
            }

            DraggingPattern = FALSE;
        }
        break;

    //
    // Block source image scrolling, whenever RMB was pressed over added patterns layer
    //

    case WM_RBUTTONDOWN:
        DragOccureInPattern = true;
        return TRUE;

    case WM_RBUTTONUP:
        DragOccureInPattern = false;
        return TRUE;

    //
    // Flip entry
    //
    case WM_LBUTTONDBLCLK:
        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            Entry->Flipped ^= 1;
            Entry->Flipped &= 1;
            InvalidateRect(Entry->Hwnd, NULL, TRUE);
            UpdateWindow(Entry->Hwnd);
        }
        break;

    case WM_PAINT:

        PERF_START("Entry WM_PAINT");

        hdc = BeginPaint(hwnd, &ps);

        EntryIndex = GetPatternEntryIndexByHwnd(hwnd);
        if (EntryIndex != -1)
        {
            Entry = &PatternLayer[EntryIndex];
            Item = PatternGetItem(Entry->PatternName);
            Rect.left = 0;
            Rect.top = 0;
            Rect.right = Entry->Width;
            Rect.bottom = Entry->Height;
            DrawPattern(Item, hdc, &Rect, Entry->Flipped ? TRUE : FALSE, TRUE, TRUE, Entry == SelectedPattern);

            //
            // Pattern remove button.
            //

            if (RemoveBitmap)
            {
                hdcMem = CreateCompatibleDC(hdc);
                oldBitmap = SelectObject(hdcMem, RemoveBitmap);

                GetObject(RemoveBitmap, sizeof(bitmap), &bitmap);
                BitBlt(hdc, Entry->Width - REMOVE_BITMAP_WIDTH, 0, REMOVE_BITMAP_WIDTH, REMOVE_BITMAP_WIDTH, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, oldBitmap);
                DeleteDC(hdcMem);
            }
        }

        EndPaint(hwnd, &ps);

        PERF_STOP("Entry WM_PAINT");

        break;

    case WM_ERASEBKGND:
        return TRUE;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

#endif

void AddPatternEntry(char * PatternName)
{
    PERF_START("AddPatternEntry");

    PatternItem * Item = PatternGetItem(PatternName);
    PatternEntry Entry;
    RECT Region;
    bool Selected = JpegGetSelectRegion(&Region);
    float LamdaWidth, LamdaHeight;
    char Text[0x100];

    if (Selected && Item)
    {
        Entry.BlendLevel = 1.0f;
        Entry.Flipped = (Button_GetCheck(FlipWnd) == BST_CHECKED);
        strcpy(Entry.PatternName, PatternName);

        //
        // Width / Height
        //

        LamdaWidth = (float)Item->PatternWidth / Item->Lamda;
        LamdaHeight = (float)Item->PatternHeight / Item->Lamda;
        Entry.Width = (int) (LamdaWidth * WorkspaceLamda);
        Entry.Height = (int) (LamdaHeight * WorkspaceLamda);

        //
        // Position
        //

        Entry.PlaneX = SelectionStartX - ScrollX;
        Entry.PlaneY = SelectionStartY - ScrollY;

        Entry.PosX = SelectionStartX;
        Entry.PosY = SelectionStartY;

        //
        // Create Window
        //

#ifndef USEGL
        Entry.Hwnd = CreateWindowEx(
            0,
            PATTERN_ENTRY_CLASS,
            "PatternEntryPopup",
            WS_OVERLAPPED | WS_CHILDWINDOW | WS_EX_LAYERED | WS_EX_NOPARENTNOTIFY,
            Entry.PosX,
            Entry.PosY,
            Entry.Width,
            Entry.Height,
            JpegWnd,
            NULL,
            GetModuleHandle(NULL),
            NULL);

        ShowWindow(Entry.Hwnd, SW_NORMAL);
        UpdateWindow(Entry.Hwnd);
#endif

        //
        // Add Entry in List.
        //

        PatternLayer = (PatternEntry *)realloc(PatternLayer, sizeof(PatternEntry)* (NumPatterns + 1));
        PatternLayer[NumPatterns++] = Entry;

        sprintf(Text, "Patterns Added : %i", NumPatterns);
        SetStatusText(STATUS_ADDED, Text);
    }

    PERF_STOP("AddPatternEntry");
}

// Update added pattern entry by some given properties (screen and layer displacement, Flip flag etc.)
void UpdatePatternEntry(int EntryIndex, PatternEntry * Entry)
{
    PERF_START("UpdatePatternEntry");

    PatternEntry * Orig = GetPatternEntry(EntryIndex);

    Orig->BlendLevel = Entry->BlendLevel;
    Orig->Flipped = Entry->Flipped;

    Orig->PosX = Entry->PosX;
    Orig->PosY = Entry->PosY;
    Orig->PlaneX = Entry->PlaneX;
    Orig->PlaneY = Entry->PlaneY;

#ifndef USEGL
    MoveWindow(Orig->Hwnd, Orig->PosX, Orig->PosY, Orig->Width, Orig->Height, TRUE);
#endif

    //InvalidateRect(Orig->Hwnd, NULL, TRUE);
    //UpdateWindow(Orig->Hwnd);

    PERF_STOP("UpdatePatternEntry");
}

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
    POINT Offset;

    switch (msg)
    {
    case WM_CREATE:

#ifdef USEGL
        /* initialize OpenGL rendering */
        hdc = GetDC(hwnd);
        SetupPixelFormat(hdc);
        hGLRC = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hGLRC);
        GL_init();
#else

        JpegOffscreenDC = CreateCompatibleDC(GetWindowDC(hwnd));
#endif  // USEGL

        break;
    case WM_CLOSE:

        DeleteDC(JpegOffscreenDC);

        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:

#ifdef USEGL
        /* finish OpenGL rendering */
        if (hGLRC) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hGLRC);
        }
#endif

        break;

    case WM_PAINT:

        PERF_START("JpegWnd WM_PAINT");

        hdc = BeginPaint(hwnd, &ps);

#ifdef USEGL

        if (hGLRC) {
            GL_redraw(hdc);
        }

#else

        //
        // Background image
        //

        if (JpegBitmap)
        {
            hdcMem = JpegOffscreenDC;
            oldBitmap = SelectObject(hdcMem, JpegBitmap);

            GetObject(JpegBitmap, sizeof(bitmap), &bitmap);
            BitBlt(hdc, ScrollX, ScrollY, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
        }

        //
        // Selection box
        //

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

#endif  // USEGL

        PERF_STOP("JpegWnd WM_PAINT");

        PerfUpdateStats(hdc);

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
        if (DragOccureInPattern == false)
        {
            ScrollingBegin = true;
            RegionSelected = false;
            SavedMouseX = LOWORD(lParam);
            SavedMouseY = HIWORD(lParam);
            SavedScrollX = ScrollX;
            SavedScrollY = ScrollY;
            SaveEntryPositions();
        }
        break;

    case WM_MOUSEMOVE:
        if (ScrollingBegin)
        {
            Offset.x = SavedScrollX + LOWORD(lParam) - SavedMouseX;
            Offset.y = SavedScrollY + HIWORD(lParam) - SavedMouseY;

            JpegSetScroll(&Offset);

            UpdateEntryPositions(LOWORD(lParam) - SavedMouseX, HIWORD(lParam) - SavedMouseY, FALSE);
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
        if (DragOccureInPattern == false)
        {
            ScrollingBegin = false;

            UpdateEntryPositions(LOWORD(lParam) - SavedMouseX, HIWORD(lParam) - SavedMouseY, FALSE);

            InvalidateRect(JpegWnd, NULL, TRUE);
            UpdateWindow(JpegWnd);

            sprintf(Text, "Scroll : %i / %ipx", ScrollX, ScrollY);
            SetStatusText(STATUS_SCROLL, Text);
        }
        DragOccureInPattern = false;
        break;

    case WM_LBUTTONUP:
        SelectionBegin = false;
        InvalidateRect(JpegWnd, NULL, TRUE);
        UpdateWindow(JpegWnd);
        RearrangePatternTiles();
        PatternRedraw();
        UpdateSelectionStatus();
        JpegSelectPattern(NULL);
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

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = JpegProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
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

#ifndef USEGL

    //
    // Register pattern window class.
    //

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc = PatternEntryProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_HAND);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = PATTERN_ENTRY_CLASS;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Cannot register Pattern EntryWnd Class", "Error", 0);
    }

#endif  // USEGL

    //
    // Load pattern remove button picture
    //

    RemoveBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_REMOVE));
}

static void JpegAddScanline(unsigned char *buffer, int stride, void *Param)
{
    memcpy((void *)(JpegBuffer + JpegBufferSize), buffer, stride);
    JpegBufferSize += stride;
}

// Load source Jpeg image
void JpegLoadImage(char *filename, bool Silent)
{
    char Text[1024];

    PERF_START("JpegLoadImage");

    SetCursor(LoadCursor(NULL, IDC_WAIT));

    JpegLoad(
        filename,
        JpegAddScanline,
        &JpegBuffer,
        &JpegBufferSize,
        &JpegWidth,
        &JpegHeight,
        NULL );

#ifdef USEGL
    //
    // GL Mesh
    //

    JpegMeshRelease(&JpegMesh);

    JpegMesh.Mesh[0] = JpegGenerateMesh(JpegWidth,
        JpegHeight,
        JpegBuffer,
        &JpegMesh.MeshWidth[0],
        &JpegMesh.MeshHeight[0],
        JPEG_MESH_LOG2,
        0);

#else

    if (JpegBitmap)
    {
        DeleteObject(JpegBitmap);
        JpegBitmap = NULL;
    }
    if (JpegBuffer)
        JpegBitmap = CreateBitmapFromPixels(GetWindowDC(JpegWnd), JpegWidth, JpegHeight, 24, JpegBuffer);

#endif  // USEGL

    sprintf(Text, "Source Image : %s", filename);
    SetStatusText(STATUS_SOURCE_IMAGE, Text);

    ScrollX = ScrollY = 0;

    if (SavedImageName)
    {
        free(SavedImageName);
        SavedImageName = NULL;
    }
    SavedImageName = (char *)malloc(strlen(filename) + 1);
    strcpy(SavedImageName, filename);
    SavedImageName[strlen(filename)] = 0;

    if (!Silent) MessageBox(0, "Loaded", "Loaded", MB_OK);

    SetCursor(LoadCursor(NULL, IDC_ARROW));

    JpegRedraw();

    PERF_STOP("JpegLoadImage");
}

// Return true and selected area, if selection box is present and large enough.
// Return false, otherwise
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

void JpegSetSelectRegion(LPRECT Region)
{
    SelectionStartX = Region->left;
    SelectionStartY = Region->top;
    SelectionEndX = Region->right;
    SelectionEndY = Region->bottom;

    RegionSelected = true;
}

// Change Jpeg window size according to parent window dimensions
void JpegResize(int Width, int Height)
{
    int winWidth, winHeight;

    winWidth = max(100, Width - 300);
    winHeight = max(100, Height - 5);

    MoveWindow(JpegWnd, 2, 2, winWidth, winHeight, TRUE);

#ifdef USEGL
    GL_resize(winWidth, winHeight);
#endif 

    // Reset selection box after window size was changed
    JpegRemoveSelection();
}

// Window width.
int JpegWindowWidth(void)
{
    RECT Rect;
    GetClientRect(JpegWnd, &Rect);
    return Rect.right;
}

// Window height.
int JpegWindowHeight(void)
{
    RECT Rect;
    GetClientRect(JpegWnd, &Rect);
    return Rect.bottom;
}

void JpegRemoveSelection(void)
{
    RegionSelected = false;
    SetStatusText(STATUS_SELECTED, "Selected: ---");
    JpegRedraw();
}

void JpegRedraw(void)
{
    PERF_START("JpegRedraw");

    InvalidateRect(JpegWnd, NULL, TRUE);
    UpdateWindow(JpegWnd);

    PERF_STOP("JpegRedraw");
}

PatternEntry * GetPatternEntry(int EntryIndex)
{
    return &PatternLayer[EntryIndex];
}

int GetPatternEntryNum(void)
{
    return NumPatterns;
}

// Destroy all system resources to create it again.
void JpegDestroy(void)
{
    JpegRemoveSelection();

    ScrollX = ScrollY = 0;

    //
    // Source image layer.
    //

#ifndef USEGL
    if (JpegBitmap)
    {
        DeleteObject(JpegBitmap);
        JpegBitmap = NULL;
    }
#endif

    //
    // Patterns layer.
    //

    JpegRemoveAllPatterns();

    JpegRedraw();
}

void JpegRemoveAllPatterns(void)
{
    int Count;
    PatternEntry *Entry;

#ifndef USEGL
    for (Count = 0; Count < NumPatterns; Count++)
    {
        Entry = GetPatternEntry(Count);
        DestroyWindow(Entry->Hwnd);
    }
#endif

    if (PatternLayer)
    {
        free(PatternLayer);
        PatternLayer = NULL;
    }
    NumPatterns = 0;

    SetStatusText(STATUS_ADDED, "Patterns Added : 0");
}

char * JpegGetImageName(bool NameOnly)
{
    if (NameOnly) return strrchr(SavedImageName, '\\') + 1;
    else return SavedImageName;
}

void JpegGetScroll(LPPOINT Offset)
{
    Offset->x = ScrollX;
    Offset->y = ScrollY;
}

void JpegSetScroll(LPPOINT Offset)
{
    char Text[0x100];

    ScrollX = Offset->x;
    ScrollY = Offset->y;

    JpegRedraw();

    sprintf(Text, "Scroll : %i / %ipx", ScrollX, ScrollY);
    SetStatusText(STATUS_SCROLL, Text);
}

void JpegSelectPattern(PatternEntry * Pattern)
{
    char Text[0x1000];
    PatternEntry * OldPattern;

    PERF_START("JpegSelectPattern");

    OldPattern = SelectedPattern;

    SelectedPattern = Pattern;

    //
    // Update newly selected Pattern
    //

    if (Pattern)
    {
#ifndef USEGL
        if (IsWindow(Pattern->Hwnd))
        {
            InvalidateRect(Pattern->Hwnd, NULL, FALSE);
            UpdateWindow(Pattern->Hwnd);
        }
#endif

        sprintf(
            Text, "Selected: %s, Plane: %i,%i, Pos: %i:%i",
            Pattern->PatternName, Pattern->PlaneX, Pattern->PlaneY, Pattern->PosX, Pattern->PosY);
        SetStatusText(STATUS_SELECTED, Text);
    }

    //
    // Update previously selected pattern
    //

#ifndef USEGL
    if (OldPattern)
    {
        if (IsWindow(OldPattern->Hwnd))
        {
            InvalidateRect(OldPattern->Hwnd, NULL, FALSE);
            UpdateWindow(OldPattern->Hwnd);
        }
    }
#endif

    PERF_STOP("JpegSelectPattern");
}

PatternEntry * JpegGetSelectedPattern(void)
{
    return SelectedPattern;
}

void JpegEnsureVisible(PatternEntry * Pattern)
{
    POINT Offset;
    int DeltaX;
    int DeltaY;

    if (Pattern == NULL) return;

    PERF_START("JpegEnsureVisible");

    DeltaX = ScrollX;
    DeltaY = ScrollY;

    SaveEntryPositions();

    //
    // Set scroll offset
    //

    Offset.x = - Pattern->PlaneX + Pattern->Width;
    Offset.y = - Pattern->PlaneY + Pattern->Height;

    JpegSetScroll(&Offset);

    //
    // Adjust added patterns windows positions, according to changed scroll offset.
    //

    DeltaX -= ScrollX;
    DeltaY -= ScrollY;

    UpdateEntryPositions(-DeltaX, -DeltaY, TRUE);

    PERF_STOP("JpegEnsureVisible");
}
