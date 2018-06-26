// Row calculation

#include "rows.h"

extern float WorkspaceLambda;
extern int WorkspaceRowIndex;

LIST_ENTRY savedRows = { &savedRows, &savedRows };

static void ClearOldList()
{
	while (!IsListEmpty(&savedRows))
	{
		LIST_ENTRY * entry = savedRows.Flink;
		RemoveEntryList(entry);
		free(entry);
	}
}

static BOOL AddRowEntry(RowEntry * add)
{
	LIST_ENTRY * entry;
	RowEntry * newEntry;
	int entrySize = sizeof(RowEntry);

	entry = (LIST_ENTRY *)malloc(entrySize);
	if (!entry)
	{
		return FALSE;
	}

	memset(entry, 0, entrySize);

	InsertTailList(&savedRows, entry);

	newEntry = (RowEntry *)entry;

	newEntry->index = add->index;
	newEntry->planeX = add->planeX;
	newEntry->planeY = add->planeY;

	return TRUE;
}

typedef struct
{
	long x;
	long width;
} XW_Pair;

int PairComparePred(const void * val1, const void * val2)
{
	XW_Pair * left = (XW_Pair *)val1;
	XW_Pair * right = (XW_Pair *)val2;

	return right->x < left->x;
}

LIST_ENTRY * RecalcRows(PatternEntry * patterns, int numPatterns)
{
	XW_Pair * pairs;

	if (!numPatterns)
	{
		return NULL;
	}

	if (!IsListEmpty(&savedRows))
	{
		ClearOldList();
	}

	//
	// Get patterns X/W pairs
	//

	pairs = (XW_Pair *)malloc(sizeof(XW_Pair) * numPatterns);
	if (!pairs)
	{
		return NULL;
	}

	for (int i = 0; i < numPatterns; i++)
	{
		pairs[i].x = patterns[i].PlaneX;
		pairs[i].width = patterns[i].Width;
	}

	//
	// Sort minmax
	//

	// WTF Bug
	//qsort(pairs, numPatterns, sizeof(XW_Pair), PairComparePred);

	for (int c = 0; c < numPatterns - 1; c++)
	{
		for (int d = 0; d < numPatterns - c - 1; d++)
		{
			if (pairs[d].x > pairs[d + 1].x) // For decreasing order use <
			{
				XW_Pair swap = pairs[d];
				pairs[d] = pairs[d + 1];
				pairs[d + 1] = swap;
			}
		}
	}

	//for (int i = 0; i < numPatterns; i++)
	//{
	//	printf("x: %i, w: %i\n", pairs[i].x, pairs[i].width);
	//}

	//printf("------------------\n");

	//
	// Remove overlapping
	//

	XW_Pair next;

	next.x = pairs[0].x;
	next.width = pairs[0].width;

	for (int i = 1; i < numPatterns; i++)
	{
		if ( pairs[i].x >= next.x && 
			pairs[i].x < ( next.x + next.width) )
		{
			pairs[i].x = 0;
		}
		else
		{
			next.x = pairs[i].x;
			next.width = pairs[i].width;
		}
	}

	//
	// Add rows
	//

	int rowIndex = WorkspaceRowIndex;

	for (int i = 0; i < numPatterns; i++)
	{
		if (pairs[i].x)
		{
			RowEntry entry;

			entry.index = rowIndex++;
			entry.planeX = pairs[i].x;
			entry.planeY = 0;

			BOOL res = AddRowEntry(&entry);

			if (!res)
			{
				free(pairs);
				return NULL;
			}
		}
	}

	free(pairs);

	return &savedRows;
}
