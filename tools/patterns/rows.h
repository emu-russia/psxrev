#pragma once

#include <Windows.h>

#include "jpegwnd.h"
#include "listutils.h"

typedef struct
{
	LIST_ENTRY entry;
	int index;
	int planeX;
	int planeY;
} RowEntry;

LIST_ENTRY * RecalcRows(PatternEntry * patterns, int numPatterns);
