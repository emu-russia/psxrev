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
extern float WorkspaceLamda, WorkspaceLamdaDelta;

void SaveWorkspace(char *filename)
{
    WorkspaceImage ws;
    char *Database;
    char *ImageName;
    POINT Offset;
    PatternEntry *PatternLayer;
    int NumPatterns;
    int Count;
    FILE * f;

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
    ws.Flipped = (Button_GetCheck(FlipWnd) == BST_CHECKED) ? true : false;

    //
    // Pattern database
    //

    Database = GetSavedDatabase();
    ws.DatabaseOffset = sizeof(WorkspaceImage);
    ws.DatabaseLength = (long)strlen(Database) + 1;

    //
    // Source image layer
    //

    JpegGetScroll(&Offset);
    ws.ScrollX = Offset.x;
    ws.ScrollY = Offset.y;

    ImageName = JpegGetImageName();
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
        fwrite(Database, 1, ws.DatabaseLength, f);      // Pattern Database
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
    char *Database;
    char *ImageName;
    POINT Offset;
    PatternEntry *PatternLayer;
    int NumPatterns;
    int Count;
    RECT Region;

    f = fopen(filename, "rb");
    if (!f)
    {
        MessageBox(0, "Cannot load workspace iamge file", "Workspace Load Failed", 0);
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
    // Stage 1 : Cleanup
    //

    //
    // Delete all system resources
    //

    JpegDestroy();
    PatternDestroy();

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
    if (ws.Flipped) Button_SetCheck(FlipWnd, BST_CHECKED);
    else Button_SetCheck(FlipWnd, BST_UNCHECKED);

    //
    // Pattern database
    //

    fseek(f, ws.DatabaseOffset, SEEK_SET);
    Database = (char *)malloc(ws.DatabaseLength);
    fread(Database, 1, ws.DatabaseLength, f);
    ParseDatabase(Database);
    PatternRedraw();

    //
    // Source image layer
    //

    if (ws.SourceImagePresent)
    {
        SetStatusText(STATUS_SOURCE_IMAGE, "Loading Image...");
        fseek(f, ws.SourceImageOffset, SEEK_SET);
        ImageName = (char *)malloc(ws.SourceImageLength);
        fread(ImageName, 1, ws.SourceImageLength, f);
        JpegLoadImage(ImageName, true);
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

    MessageBox(0, "Workspace Loaded", "Workspace Loaded", 0);
}

// Add periodic handler, which will save workspace every Some seconds into WORKSPACE_AUTOSAVE file.
void EnableAutosave(int Seconds)
{

}
