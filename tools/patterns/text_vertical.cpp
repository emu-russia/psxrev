// Vertical patterns text saver.

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include "jpegwnd.h"

//
// Debug visualisation
//

//#define DEBUG_VIS
#define DEBUG_VIS_DELAY  200        // ms

//
// Max number of pattern columns in source image.
//

#define MAX_COLUMNS 1024

typedef struct _TEXT_ENTRY
{
    LIST_ENTRY  ListEntry;

    PatternEntry * Pattern;

} TEXT_ENTRY, *PTEXT_ENTRY;

static void InitializeListHead(PLIST_ENTRY ListHead)
{
    ListHead->Blink = ListHead->Flink = ListHead;
}

static void InsertTailList(PLIST_ENTRY ListHead, PLIST_ENTRY Entry)
{
    PLIST_ENTRY OldBlink;
    OldBlink = ListHead->Blink;
    Entry->Flink = ListHead;
    Entry->Blink = OldBlink;
    OldBlink->Flink = Entry;
    ListHead->Blink = Entry;
}

static void RemoveEntryList(PLIST_ENTRY Entry)
{
    PLIST_ENTRY OldFlink;
    PLIST_ENTRY OldBlink;

    OldFlink = Entry->Flink;
    OldBlink = Entry->Blink;
    OldFlink->Blink = OldBlink;
    OldBlink->Flink = OldFlink;
}

static BOOLEAN IsListEmpty(PLIST_ENTRY ListHead)
{
    return ListHead->Flink == ListHead;
}

// Just enumerate added patterns layer.
static void DebugEnum(PatternEntry *List, int Count, FILE *f)
{
    int n;

    for (n = 0; n < Count; n++)
    {
        fprintf( f, 
            "Pattern %i : name=%s, posx:%i, posy:%i\n",
            n, List[n].PatternName, List[n].PlaneX, List[n].PlaneY );
    }
}

// Check if PatternEntry is already in columns list
static BOOLEAN AlreadyInList(PatternEntry *Pattern, PTEXT_ENTRY ListHead)
{
    PTEXT_ENTRY TextEntry;

    TextEntry = (PTEXT_ENTRY)ListHead->ListEntry.Flink;

    while (TextEntry != (PTEXT_ENTRY)ListHead)
    {
        if (TextEntry->Pattern == Pattern) return TRUE;

        TextEntry = (PTEXT_ENTRY)TextEntry->ListEntry.Flink;
    }

    return FALSE;
}

// Locate pattern within rect(posx,posy,AvgWidth,MinHeight)
static PatternEntry * Locate(PatternEntry *List, int Count, int PosX, int PosY, int Width, int Height, FILE *f)
{
    int n;
    bool FitWidth;
    bool FitHeight;

    //fprintf(f, "Matching against rect(%i,%i,%i,%i)\n", PosX, PosY, Width, Height);

    for (n = 0; n < Count; n++)
    {
        FitWidth = List[n].PlaneX >= PosX && List[n].PlaneX < (PosX + Width);

        FitHeight = List[n].PlaneY >= PosY && List[n].PlaneY < (PosY + Height);

        if (FitWidth && FitHeight) return &List[n];
    }

    return NULL;
}

void TextSaverVertical (PatternEntry *List, int Count, char *Filename)
{
    FILE * f;
    PatternEntry * Leftmost;
    PatternEntry * Rightmost;
    PatternEntry * Topmost;
    PatternEntry * Bottomost;
    PatternEntry * Pattern;
    int AvgWidth;
    int MinHeight;
    int n;
    int posx, posy;
    PTEXT_ENTRY ListHeads;
    PTEXT_ENTRY CurrentHead;
    PTEXT_ENTRY TextEntry;
    int NumListHeads;
    int PatternsFound;
    int MaxNameLength;
    int NameLength;
    int SavedPosx;
    int MaxWidth;
    int NewPosx;

    if (Count == 0) return;

    //
    // Create output file
    //
    
    f = fopen(Filename, "wt");

    if (f)
    {
        //DebugEnum(List, Count, f);

        //
        // Initialize columns list
        //

        ListHeads = (PTEXT_ENTRY)malloc(sizeof(TEXT_ENTRY) * MAX_COLUMNS);

        CurrentHead = &ListHeads[0];
        CurrentHead->Pattern = NULL;

        InitializeListHead(&CurrentHead->ListEntry);

        NumListHeads = 1;

        //
        // Find leftmost, rightmost, topmost patterns and calculate average width and min height.
        //

        MinHeight = 999999;

        AvgWidth = 0;

        Leftmost = Topmost = Rightmost = Bottomost = &List[0];

        MaxNameLength = 0;

        for (n = 0; n < Count; n++)
        {
            AvgWidth += List[n].Width;

            if (List[n].PlaneX < Leftmost->PlaneX) Leftmost = &List[n];

            if (List[n].PlaneX > Rightmost->PlaneX) Rightmost = &List[n];

            if (List[n].PlaneY < Topmost->PlaneY) Topmost = &List[n];

            if (List[n].PlaneY > Bottomost->PlaneY) Bottomost = &List[n];

            if (List[n].Height < MinHeight) MinHeight = List[n].Height;
            
            NameLength = strlen(List[n].PatternName);
            if (NameLength > MaxNameLength) MaxNameLength = NameLength;
        }

        AvgWidth /= Count;

        //
        // Debug
        //

#if 0
        fprintf(f, "Lefmost cell: %s, posx=%i\n", Leftmost->PatternName, Leftmost->PlaneX);
        fprintf(f, "Rightmost cell: %s, posx=%i\n", Rightmost->PatternName, Rightmost->PlaneX);
        fprintf(f, "Topmost cell: %s, posy=%i\n", Topmost->PatternName, Topmost->PlaneX);
        fprintf(f, "Bottomost cell: %s, posy=%i\n", Bottomost->PatternName, Bottomost->PlaneY);
        fprintf(f, "Average cell width: %i\n", AvgWidth);
        fprintf(f, "Min cell height: %i\n", MinHeight);
#endif

        //
        // Iterate columns from Leftmost posx, with step = AvgWidth, until posx < (Rightmost posx + Rightmost Width)
        //

        for (posx = Leftmost->PlaneX; posx < (Rightmost->PlaneX + Rightmost->Width); )
        {
            //
            // Iterate rows from Topmost posy, with step = Cell Height, until posy < (Bottomost posy + Bottomost Height)
            //

            PatternsFound = 0;

            //fprintf(f, "COLUMN:\n");

            MaxWidth = 0;

            for (posy = Topmost->PlaneY; posy < (Bottomost->PlaneY + Bottomost->Height);)
            {
                //
                // Locate pattern within rect(posx,posy,AvgWidth,MinHeight)
                //

                Pattern = Locate(List, Count, posx, posy, AvgWidth, MinHeight, f);

                //
                // Add pattern to columns list
                //

                if (Pattern)
                {
                    if (!AlreadyInList(Pattern, CurrentHead))
                    {
                        //fprintf(f, "%s: posx=%i, posy=%i\n", Pattern->PatternName, Pattern->PlaneX, Pattern->PlaneY);

                        TextEntry = (PTEXT_ENTRY)malloc(sizeof(TEXT_ENTRY));

                        TextEntry->Pattern = Pattern;

                        InsertTailList(&CurrentHead->ListEntry, (PLIST_ENTRY)&TextEntry->ListEntry);

                        PatternsFound++;

                        if ( Pattern->Width > MaxWidth ) MaxWidth = Pattern->Width;

#ifdef DEBUG_VIS
                        JpegSelectPattern(Pattern);
                        JpegEnsureVisible(Pattern);

                        Sleep(DEBUG_VIS_DELAY);
#endif // DEBUG_VIS
                    }
                }

                //
                // Adjust posy
                //

                if ( Pattern ) posy += Pattern->Height;
                else posy += MinHeight;
            }

            //
            // Recalculate Leftmost posx
            //

            SavedPosx = posx;

            NewPosx = Rightmost->PlaneX;

            for (n = 0; n < Count; n++)
            {
                if (List[n].PlaneX > (SavedPosx + MaxWidth))
                {
                    if (List[n].PlaneX < NewPosx) NewPosx = List[n].PlaneX;
                }
            }

            posx = NewPosx;

            if (posx == SavedPosx) break;

            //
            // Advance list heads
            //

            if (PatternsFound)
            {
                CurrentHead = &ListHeads[NumListHeads++];

                CurrentHead->Pattern = NULL;

                InitializeListHead(&CurrentHead->ListEntry);

                if (NumListHeads >= MAX_COLUMNS) break;
            }

            //fprintf(f, "\n");
        }

        //
        // Debug: Dump column lists
        //

#if 1
        for (n = 0; n < NumListHeads; n++)
        {
            CurrentHead = &ListHeads[n];

            TextEntry = (PTEXT_ENTRY)CurrentHead->ListEntry.Flink;

            fprintf(f, "Column %i:\n", n);

            while (TextEntry != (PTEXT_ENTRY)CurrentHead)
            {
                if (TextEntry->Pattern)
                {
                    fprintf(f, "%s\n", TextEntry->Pattern->PatternName);
                }

                TextEntry = (PTEXT_ENTRY)TextEntry->ListEntry.Flink;
            }

            fprintf(f, "\n");
        }
#endif

        //
        // Cleanup.
        //

        for (n = 0; n < NumListHeads; n++)
        {
            CurrentHead = &ListHeads[n];

            while (!IsListEmpty(&CurrentHead->ListEntry))
            {
                TextEntry = (PTEXT_ENTRY)CurrentHead->ListEntry.Flink;

                RemoveEntryList(&TextEntry->ListEntry);

                free(TextEntry);
            }
        }

        free(ListHeads);

        fclose(f);

#ifdef DEBUG_VIS
        JpegSelectPattern(NULL);
#endif // DEBUG_VIS
    }
}
