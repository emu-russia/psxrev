#pragma once

void MapInit(HWND Parent);

void MapResize(int Width, int Height);

void MapRedraw(void);

void MapUpdate();

void MapGetDims(LPRECT mapWnd);

void MapScroll(int x, int y);

void MapPlaneToMap(RECT * planeBox, RECT *mapBox);
