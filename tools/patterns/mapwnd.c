#define _CRT_SECURE_NO_WARNINGS

// Simple mini-map window

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <Windows.h>
#include <windowsx.h>

#include "jpegwnd.h"
#include "mapwnd.h"

static HWND ParentWnd;
static HWND MapWnd;

static HPEN MapPen;

#define MAP_WND_SIZE  ( JpegWindowWidth() / 5 )

#define MAP_WND_X  ( JpegWindowWidth() - MAP_WND_SIZE )
#define MAP_WND_Y  ( 2 )

LRESULT CALLBACK MapWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void MapInit(HWND Parent)
{
    WNDCLASSEX wc;
    RECT Rect;

	//
	// Create pen
	//

	MapPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

    ParentWnd = Parent;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = MapWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "MapWnd";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Cannot register MapWnd Class", "Error", 0);
        return;
    }

    GetClientRect(ParentWnd, &Rect);

    MapWnd = CreateWindowEx(
        0,
        "MapWnd",
        "MapWndPopup",
        WS_OVERLAPPED | WS_CHILDWINDOW,
        2,
        2,
        MAP_WND_SIZE,
        MAP_WND_SIZE,
        ParentWnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(MapWnd, SW_NORMAL);
    UpdateWindow(MapWnd);

	MapUpdate();

	if (JpegWindowWidth() < 2*MAP_WND_SIZE ||
		JpegWindowHeight() < 2*MAP_WND_SIZE )
	{
		ShowWindow(MapWnd, SW_HIDE);
	}
}

void MapResize(int Width, int Height)
{
    if (JpegWindowWidth() < 2*MAP_WND_SIZE ||
        JpegWindowHeight() < 2*MAP_WND_SIZE )
    {
		ShowWindow(MapWnd, SW_HIDE);
        return;
    }

	ShowWindow(MapWnd, SW_NORMAL);

    MoveWindow( MapWnd,
		MAP_WND_X,
		MAP_WND_Y,
        MAP_WND_SIZE,
        MAP_WND_SIZE,
        TRUE);
}

void MapRedraw(void)
{
    //InvalidateRect(MapWnd, NULL, TRUE);
    //UpdateWindow(MapWnd);
}

void MapUpdate()
{
	POINT jpegSize;
	POINT scroll;
	POINT jpegWndSize;
    HDC mapDc = GetDC(MapWnd);

	SelectObject(mapDc, MapPen);

	JpegGetDims(&jpegSize);
	JpegGetScroll(&scroll);

	if (!jpegSize.y)
	{
		ReleaseDC(MapWnd, mapDc);
		return;
	}

	jpegWndSize.x = JpegWindowWidth();
	jpegWndSize.y = JpegWindowHeight();

	int mapWidth;
	int mapHeight;

	if (jpegSize.x > jpegSize.y)
	{
		float aspect = (float)jpegSize.x / (float)jpegSize.y;

		mapWidth = MAP_WND_SIZE;
		mapHeight = (int)((float)MAP_WND_SIZE / aspect);
	}
	else
	{
		float aspect = (float)jpegSize.y / (float)jpegSize.x;

		mapWidth = (int)((float)MAP_WND_SIZE / aspect);
		mapHeight = MAP_WND_SIZE;
	}

	//
	// Draw image border
	//

	MoveToEx(mapDc, 0, 0, NULL);
	LineTo(mapDc, 0, mapHeight);
	LineTo(mapDc, mapWidth, mapHeight);
	LineTo(mapDc, mapWidth, 0);
	LineTo(mapDc, 0, 0);

	//
	// Draw current display scroll offset
	//

	int scrollX = (int)((float)-scroll.x / ( (float)jpegSize.x / (float)mapWidth ));
	int scrollY = (int)((float)-scroll.y / ((float)jpegSize.y / (float)mapHeight ));

	int displayW = (int)((float)jpegWndSize.x / ((float)jpegSize.x / (float)mapWidth));
	int displayH = (int)((float)jpegWndSize.y / ((float)jpegSize.y / (float)mapHeight));

	MoveToEx(mapDc, scrollX, scrollY, NULL);
	LineTo(mapDc, scrollX, scrollY + displayH);
	LineTo(mapDc, scrollX + displayW, scrollY + displayH);
	LineTo(mapDc, scrollX + displayW, scrollY);
	LineTo(mapDc, scrollX, scrollY);

	ReleaseDC(MapWnd, mapDc);
}

void MapGetDims(LPRECT mapWnd)
{
	POINT jpegSize;

	JpegGetDims(&jpegSize);

	if (jpegSize.y == 0 || !IsWindowVisible(MapWnd))
	{
		mapWnd->right = mapWnd->bottom = 0;
		return;
	}

	int mapWidth;
	int mapHeight;

	if (jpegSize.x > jpegSize.y)
	{
		float aspect = (float)jpegSize.x / (float)jpegSize.y;

		mapWidth = MAP_WND_SIZE;
		mapHeight = (int)((float)MAP_WND_SIZE / aspect);
	}
	else
	{
		float aspect = (float)jpegSize.y / (float)jpegSize.x;

		mapWidth = (int)((float)MAP_WND_SIZE / aspect);
		mapHeight = MAP_WND_SIZE;
	}

	mapWnd->left = MAP_WND_X;
	mapWnd->top = MAP_WND_Y;
	mapWnd->right = mapWnd->left + mapWidth - 1;
	mapWnd->bottom = mapWnd->top + mapHeight - 1;
}

//
// Translate pointing device hit test to JpegWnd scroll change
//

void MapScroll(int x, int y)
{
	POINT jpegSize;

	JpegGetDims(&jpegSize);

	if (jpegSize.y == 0 || !IsWindowVisible(MapWnd))
	{
		return;
	}

	int mapWidth;
	int mapHeight;

	if (jpegSize.x > jpegSize.y)
	{
		float aspect = (float)jpegSize.x / (float)jpegSize.y;

		mapWidth = MAP_WND_SIZE;
		mapHeight = (int)((float)MAP_WND_SIZE / aspect);
	}
	else
	{
		float aspect = (float)jpegSize.y / (float)jpegSize.x;

		mapWidth = (int)((float)MAP_WND_SIZE / aspect);
		mapHeight = MAP_WND_SIZE;
	}

	int scrollX = -(int)((float)x * ((float)jpegSize.x / (float)mapWidth));
	int scrollY = -(int)((float)y * ((float)jpegSize.y / (float)mapHeight));

	JpegGoto(scrollX, scrollY);
}
