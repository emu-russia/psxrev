// Integrated profiler.

// How does it work.
// 1. Register profiler entities in list
// 2. Encapsulate PerfStart / PerfStop in corresponding procedures to collect stats
// 3. Update procedure stats periodically on screen as overlay strings

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include "profiler.h"
#include "jpegwnd.h"
#include "listutils.h"

static BOOLEAN PerfActive;

static HFONT PerfFont;

#define FONT_HEIGHT 13

static LIST_ENTRY PerfHead =
{
    &PerfHead,
    &PerfHead
};

static BOOLEAN PerfWorkerExitFlag;

BOOLEAN PerfRunning(void)
{
    return PerfActive;
}

LARGE_INTEGER PerfTotalExecutionTime(void)
{
    PPROFILER_ENTRY Entry;
    LARGE_INTEGER Z;
    LARGE_INTEGER One;

    Z.QuadPart = 0;
    One.QuadPart = 1;

    Entry = (PPROFILER_ENTRY)PerfHead.Flink;

    while (Entry != (PPROFILER_ENTRY)&PerfHead)
    {
        Z.QuadPart += Entry->ExecutionTime.QuadPart;

        Entry = (PPROFILER_ENTRY)Entry->ListEntry.Flink;
    }

    return Z.QuadPart ? Z : One;
}

DWORD WINAPI PerfWorkerThread(LPVOID lpParameter)
{
    PPROFILER_ENTRY Entry;
    LARGE_INTEGER TotalTime;

    //
    // Reset execution times
    //

    while (PerfWorkerExitFlag == FALSE)
    {
        TotalTime = PerfTotalExecutionTime();

        Entry = (PPROFILER_ENTRY)PerfHead.Flink;

        while (Entry != (PPROFILER_ENTRY)&PerfHead)
        {
            Entry->ExecutionTime.QuadPart = 0;

            Entry = (PPROFILER_ENTRY)Entry->ListEntry.Flink;
        }

        if (TotalTime.QuadPart != 1) JpegRedraw();

        Sleep(1000);
    }

    return 0;
}

void PerfInit(void)
{
    LOGFONT LogFont;

    if (PerfActive) return;

    memset(&LogFont, 0, sizeof(LogFont));

    strcpy(LogFont.lfFaceName, "Calibri");
    LogFont.lfWeight = FW_BOLD;
    LogFont.lfEscapement = 0;
    LogFont.lfHeight = FONT_HEIGHT;

    PerfFont = CreateFontIndirect(&LogFont);

    PerfWorkerExitFlag = FALSE;

    CreateThread(NULL, 0, PerfWorkerThread, NULL, 0, NULL);

    PerfActive = TRUE;
    JpegRedraw();
}

void PerfShutdown(void)
{
    if (!PerfActive) return;
    PerfActive = FALSE;

    if (PerfFont)
        DeleteObject(PerfFont);

    PerfWorkerExitFlag = TRUE;
    
    JpegRedraw();
}

void PerfRegisterEntity(char *ProcName)
{
    PPROFILER_ENTRY  Entry;

    Entry = (PPROFILER_ENTRY)malloc(sizeof(PROFILER_ENTRY));

    memset(Entry, 0, sizeof(PROFILER_ENTRY));

    strncpy(Entry->ProcName, ProcName, sizeof(Entry->ProcName) - 1);

    InsertTailList(&PerfHead, (PLIST_ENTRY)Entry);
}

PPROFILER_ENTRY PerfGetEntry(char *ProcName)
{
    PPROFILER_ENTRY Entry;

    Entry = (PPROFILER_ENTRY)PerfHead.Flink;

    while (Entry != (PPROFILER_ENTRY)&PerfHead)
    {
        if (!strcmp(Entry->ProcName, ProcName)) return Entry;

        Entry = (PPROFILER_ENTRY)Entry->ListEntry.Flink;
    }

    return NULL;
}

void PerfStart(char *ProcName)
{
    PPROFILER_ENTRY Entry;

    Entry = PerfGetEntry(ProcName);

    if (Entry && PerfActive)
    {
        QueryPerformanceCounter(&Entry->StartTime);
    }
}

void PerfStop(char *ProcName)
{
    PPROFILER_ENTRY Entry;

    Entry = PerfGetEntry(ProcName);

    if (Entry && PerfActive)
    {
        QueryPerformanceCounter(&Entry->StopTime);

        Entry->ExecutionTime.QuadPart += Entry->StopTime.QuadPart - Entry->StartTime.QuadPart;
    }
}

void PerfUpdateStats(HDC hdc)
{
    char Text[1024];
    int TextLength;
    PPROFILER_ENTRY Entry;
    int y;
    LARGE_INTEGER TotalTime;

    if (!PerfActive) return;

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    if (PerfFont) SelectObject(hdc, PerfFont);

    //
    // Calc total execution time
    //

    TotalTime = PerfTotalExecutionTime();

    //
    // Enumerate all profiler entities
    //

    y = 0;

    Entry = (PPROFILER_ENTRY)PerfHead.Flink;

    while (Entry != (PPROFILER_ENTRY)&PerfHead)
    {
        TextLength = sprintf(Text, "%s: %.2f%%",
            Entry->ProcName,
            ((float)Entry->ExecutionTime.QuadPart * 100.0f) / (float)TotalTime.QuadPart );

        TextOut(hdc, 0, y * FONT_HEIGHT, Text, TextLength);

        Entry = (PPROFILER_ENTRY)Entry->ListEntry.Flink;

        y++;
    }
}
