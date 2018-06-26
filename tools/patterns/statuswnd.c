#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "statuswnd.h"

HWND StatusWnd = NULL;

extern float WorkspaceLambda, WorkspaceLambdaDelta;
extern int WorkspaceRowIndex;

// set default values of statusbar parts
void ResetStatusBar(void)
{
    char LambdaDelta[256];
    
    sprintf(LambdaDelta, "Lambda / Delta / Row : %.1f / %.1f / %i",
		WorkspaceLambda, WorkspaceLambdaDelta, WorkspaceRowIndex);

    SetStatusText(STATUS_SELECTED, "Selected: ---");
    SetStatusText(STATUS_SCROLL, "Scroll : 0 / 0px");
    SetStatusText(STATUS_PATTERNS, "Patterns : 0 / 0");
    SetStatusText(STATUS_ADDED, "Added : 0");
    SetStatusText(STATUS_LAMBDA_DELTA, LambdaDelta);
    SetStatusText(STATUS_SOURCE_IMAGE, "Not Loaded");
}

// create status bar window
void CreateStatusBar(HWND Parent)
{
    int parts[] = { 0, 350, 500, 650, 800, 1000, -1 };

    if (Parent == NULL) return;

    // create window
    StatusWnd = CreateStatusWindow(
        WS_CHILD | WS_VISIBLE,
        NULL,
        Parent,
        ID_STATUS_BAR
        );

    // depart statusbar
    SendMessage(StatusWnd,
        SB_SETPARTS,
        (WPARAM)sizeof(parts) / sizeof(int),
        (LPARAM)parts);

    // set default values
    ResetStatusBar();
}

// change text in specified statusbar part
void SetStatusText(int sbPart, char *text)
{
    if (StatusWnd == NULL) return;
    SendMessage(StatusWnd, SB_SETTEXT, (WPARAM)(sbPart), (LPARAM)text);
}

int GetStatusBarHeight(void)
{
    RECT Rect;
    if (IsWindow(StatusWnd))
    {
        GetClientRect(StatusWnd, &Rect);
        return Rect.bottom;
    }
    else return 0;
}

void ResizeStatusBar(int Width, int Height)
{
    int StatusBarHeight = GetStatusBarHeight();

    if (IsWindow(StatusWnd))
    {
        MoveWindow(StatusWnd, 0, Height - StatusBarHeight, Width, StatusBarHeight, TRUE);
        InvalidateRect(StatusWnd, NULL, TRUE);
        UpdateWindow(StatusWnd);
    }
}
