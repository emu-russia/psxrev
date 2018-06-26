#pragma once

/*
Selected: 100 / 100px, 3.4 / 5.5l / ---
Scroll : 100 / 100px
Patterns : Shown / 100
Added : 5
Lambda / Delta / Row : 6.0 / 3.0 / 0
Source Image : Path / Not Loaded(last)
*/

// Status Bar Parts
enum {
    STATUS_SELECTED = 1,
    STATUS_SCROLL,
    STATUS_PATTERNS,
    STATUS_ADDED,
    STATUS_LAMBDA_DELTA,
    STATUS_SOURCE_IMAGE,
};

void CreateStatusBar(HWND Parent);

void ResetStatusBar(void);

void SetStatusText(int sbPart, char *text);

int GetStatusBarHeight(void);

void ResizeStatusBar(int Width, int Height);
