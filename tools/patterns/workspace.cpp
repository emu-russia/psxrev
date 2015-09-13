// Workspace Backup (saving/loading)
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>
#include "jpegwnd.h"
#include "patternwnd.h"
#include "statuswnd.h"
#include "workspace.h"

extern HWND FlipWnd;
extern HWND MirrorWnd;
extern float WorkspaceLamda, WorkspaceLamdaDelta;

char * FileSmartSize(ULONG size);

void SaveWorkspace(char *filename)
{
    WorkspaceImage ws;
    char *ImageName;
    POINT Offset;
    PatternEntry *PatternLayer;
    int NumPatterns;
    int Count;
    FILE * f;
    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char Name[MAX_PATH];
    char WorkspacePath[MAX_PATH];

    //
    // Obtain workspace path
    //

    _splitpath(filename, Drive, Dir, Name, NULL);
    sprintf(WorkspacePath, "%s%s", Drive, Dir);

    //
    // Stage 1 : Save context.
    //

    strcpy(ws.Signature, "WRK");
    ws.Signature[3] = 0;

    //
    // Global settings
    //

    ws.Lamda = WorkspaceLamda;
    ws.LamdaDelta = WorkspaceLamdaDelta;
    ws.Flag = 0;
    if (Button_GetCheck(FlipWnd) == BST_CHECKED)
        ws.Flag |= FLAG_FLIP;
    if (Button_GetCheck(MirrorWnd) == BST_CHECKED)
        ws.Flag |= FLAG_MIRROR;

    //
    // Pattern database - deprecated
    //

    ws.DatabaseOffset = 0;
    ws.DatabaseLength = 0;

    //
    // Source image layer
    //

    JpegGetScroll(&Offset);
    ws.ScrollX = Offset.x;
    ws.ScrollY = Offset.y;

    ImageName = JpegGetImageName(true);
    if (ImageName)
    {
        ws.SourceImagePresent = true;
        ws.SourceImageOffset = sizeof(WorkspaceImage)+ws.DatabaseLength;
        ws.SourceImageLength = (long)strlen(ImageName) + 1;
    }
    else
    {
        ws.SourceImagePresent = false;
        ws.SourceImageOffset = 0;
        ws.SourceImageLength = 0;
    }

    //
    // Added patterns layer
    //

    NumPatterns = GetPatternEntryNum();
    PatternLayer = (PatternEntry *)malloc(sizeof(PatternEntry) * NumPatterns);

    for (Count = 0; Count < NumPatterns; Count++)
    {
        memcpy(&PatternLayer[Count], GetPatternEntry(Count), sizeof(PatternEntry));

        // Cleanup not needed fields
        PatternLayer[Count].Hwnd = NULL;
        PatternLayer[Count].SavedPosX = PatternLayer[Count].SavedPosY = 0;
    }

    ws.PatternsAdded = NumPatterns;
    if (ws.PatternsAdded) ws.PatternsLayerOffset = sizeof(WorkspaceImage) + ws.DatabaseLength + ws.SourceImageLength;
    else ws.PatternsLayerOffset = 0;

    //
    // Save workspace in file
    //

    f = fopen(filename, "wb");
    if (f)
    {
        fwrite(&ws, 1, sizeof(WorkspaceImage), f);      // Header
        fwrite(ImageName, 1, ws.SourceImageLength, f);      // Source image layer
        fwrite(PatternLayer, 1, sizeof(PatternEntry)* NumPatterns, f);   // Added patterns layer
        fclose(f);
    }
    else MessageBox(0, "Cannot create workspace iamge file", "Workspace Save Failed", 0);
}

void LoadWorkspace(char *filename)
{
    WorkspaceImage ws;
    FILE * f;
    char Text[0x100];
    char *ImageName;
    POINT Offset;
    PatternEntry *PatternLayer;
    int NumPatterns;
    int Count;
    RECT Region;
    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char Name[MAX_PATH];
    char WorkspacePath[MAX_PATH];
    char ImagePath[MAX_PATH];
    ULONG JpegSize;
    CHAR JpegInfo[0x200];

    f = fopen(filename, "rb");
    if (!f)
    {
        MessageBox(0, "Cannot load workspace image file", "Workspace Load Failed", 0);
        return;
    }

    fread(&ws, 1, sizeof(WorkspaceImage), f);
    if ( strcmp(ws.Signature, "WRK"))
    {
        fclose(f);
        MessageBox(0, "Incorrect workspace image file format", "Workspace Load Failed", 0);
        return;
    }

    //
    // Obtain workspace path
    //

    _splitpath(filename, Drive, Dir, Name, NULL);
    sprintf(WorkspacePath, "%s%s", Drive, Dir);

    //
    // Stage 1 : Cleanup
    //

    //
    // Delete all system resources
    //

    JpegDestroy();

    //
    // Set default Lamda / Delta.
    //

    WorkspaceLamda = WorkspaceLamdaDelta = 1.0f;

    //
    // Set default status line.
    //

    ResetStatusBar ();

    //
    // Stage 2 : Load.
    //

    //
    // Global settings
    //

    WorkspaceLamda = ws.Lamda;
    WorkspaceLamdaDelta = ws.LamdaDelta;
    if (ws.Flag & FLAG_FLIP) Button_SetCheck(FlipWnd, BST_CHECKED);
    else Button_SetCheck(FlipWnd, BST_UNCHECKED);
    if (ws.Flag & FLAG_MIRROR) Button_SetCheck(MirrorWnd, BST_CHECKED);
    else Button_SetCheck(MirrorWnd, BST_UNCHECKED);

    //
    // Source image layer
    //

    if (ws.SourceImagePresent)
    {
        SetStatusText(STATUS_SOURCE_IMAGE, "Loading Image...");
        fseek(f, ws.SourceImageOffset, SEEK_SET);
        ImageName = (char *)malloc(ws.SourceImageLength);
        fread(ImageName, 1, ws.SourceImageLength, f);
        sprintf(ImagePath, "%s%s", WorkspacePath, ImageName);
        JpegSize = JpegLoadImage(ImagePath, true);
    }

    Offset.x = ws.ScrollX;
    Offset.y = ws.ScrollY;
    JpegSetScroll(&Offset);

    //
    // Added patterns layer
    //

    NumPatterns = ws.PatternsAdded;
    if (NumPatterns)
    {
        PatternLayer = (PatternEntry *)malloc(sizeof(PatternEntry)* NumPatterns);

        fseek(f, ws.PatternsLayerOffset, SEEK_SET);
        fread(PatternLayer, 1, sizeof(PatternEntry)* NumPatterns, f);

        for (Count = 0; Count < NumPatterns; Count++)
        {
            Region.left = PatternLayer[Count].PosX;
            Region.top = PatternLayer[Count].PosY;
            Region.right = Region.left + PatternLayer[Count].Width;
            Region.bottom = Region.top + PatternLayer[Count].Height;

            JpegSetSelectRegion(&Region);

            AddPatternEntry(PatternLayer[Count].PatternName);
            UpdatePatternEntry(Count, &PatternLayer[Count]);

            JpegRemoveSelection();
        }
    }

    JpegSelectPattern(NULL);

    fclose(f);

    //
    // Update status line
    //

    sprintf(Text, "Lamda / Delta : %.1f / %.1f", WorkspaceLamda, WorkspaceLamdaDelta);
    SetStatusText(STATUS_LAMDA_DELTA, Text);

    sprintf(JpegInfo, "JpegBufferSize: %s", FileSmartSize(JpegSize));
    MessageBox(0, JpegInfo, "Workspace Loaded", 0);
}
