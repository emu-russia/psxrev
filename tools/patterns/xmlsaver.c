// XML Format.

/*

<?xml version="1.0"?>
<ArrayOfEntity xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Entity>
    <Label>DFF_15</Label>
    <LambdaWidth>21.1000366</LambdaWidth>
    <LambdaHeight>30.2999954</LambdaHeight>
    <LambdaX>816.8938</LambdaX>
    <LambdaY>123.4741</LambdaY>
    <LambdaEndX>1</LambdaEndX>
    <LambdaEndY>1</LambdaEndY>
    <Type>CellFlipFlop</Type>
    <ColorOverride>Black</ColorOverride>
    <LabelAlignment>GlobalSettings</LabelAlignment>
    <Priority>1</Priority>
  </Entity>

  ...

</ArrayOfEntity>

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#include "patternwnd.h"
#include "jpegwnd.h"

extern float WorkspaceLamda;

#define CELL_PRIORITY 1
#define VIAS_PRIORITY 3

static BOOL RemoveAmpersands(char * string)
{
    BOOL Result = FALSE;
    char * ptr = string;

    if (string == NULL)
        return FALSE;

    while (*ptr)
    {
        if (*ptr == '&')
        {
            *ptr = 'n';
            Result = TRUE;
        }
        ptr++;
    }

    return Result;
}

void XmlSave ( char * FileName )
{
    int i, NumCells;
    PatternEntry * Pattern;
    PatternItem * Item;
    FILE *f;
    PViasCollectionEntry Coll;
    PLIST_ENTRY Entry;
    PViasEntry Vias;
    float CellPosX, CellPosY;
    float CellWidth, CellHeight;
    float ViasPosX, ViasPosY;
    BOOL Ampersand = FALSE;
    BOOL Result;
    CHAR Name[0x100];

    f = fopen ( FileName, "wt" );
    if ( !f )
        return;

    fprintf ( f, "<?xml version=\"1.0\"?>\n"
                 "<ArrayOfEntity xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n" );

    NumCells = GetPatternEntryNum ();

    for (i=0; i<NumCells; i++ )
    {
        Pattern = GetPatternEntry(i);

        Item = PatternGetItem ( Pattern->PatternName );

        //
        // Cell
        //

        CellPosX = (float)Pattern->PlaneX / WorkspaceLamda;
        CellPosY = (float)Pattern->PlaneY / WorkspaceLamda;
        CellWidth = (float)Pattern->Width / WorkspaceLamda;
        CellHeight = (float)Pattern->Height / WorkspaceLamda;

        strcpy(Name, Pattern->PatternName);
        Result = RemoveAmpersands(Name);
        if (Result && !Ampersand)
            Ampersand = TRUE;

        fprintf ( f, "  <Entity>\n" );
        fprintf ( f, "    <Label>%s</Label>\n", Name);
        fprintf ( f, "    <LambdaWidth>%f</LambdaWidth>\n", CellWidth );
        fprintf ( f, "    <LambdaHeight>%f</LambdaHeight>\n", CellHeight );
        fprintf ( f, "    <LambdaX>%f</LambdaX>\n", CellPosX );
        fprintf ( f, "    <LambdaY>%f</LambdaY>\n", CellPosY );
        fprintf ( f, "    <LambdaEndX>1</LambdaEndX>\n" );
        fprintf ( f, "    <LambdaEndY>1</LambdaEndY>\n" );

        fprintf ( f, "    <Type>" );
        switch ( Item->Type )
        {
            case CellNot:
                fprintf ( f, "CellNot" );
                break;
            case CellBuffer:
                fprintf ( f, "CellBuffer" );
                break;
            case CellMux:
                fprintf ( f, "CellMux" );
                break;
            case CellLogic:
                fprintf ( f, "CellLogic" );
                break;
            case CellAdder:
                fprintf ( f, "CellAdder" );
                break;
            case CellBusSupp:
                fprintf ( f, "CellBusSupp" );
                break;
            case CellFlipFlop:
                fprintf ( f, "CellFlipFlop" );
                break;
            case CellLatch:
                fprintf ( f, "CellLatch" );
                break;
            case CellOther:
                fprintf ( f, "CellOther" );
                break;
        }
        fprintf ( f, "</Type>\n" );

        fprintf ( f, "    <ColorOverride>Black</ColorOverride>\n" );

        fprintf ( f, "    <LabelAlignment>" );
        switch ( Pattern->Flag & 3 )
        {
            case 0:         // Normal
                fprintf ( f, "TopLeft" );
                break;
            case 1:         // Flipped
                fprintf ( f, "BottomRight" );
                break;
            case 2:         // Mirror
                fprintf ( f, "TopRight" );
                break;
            case 3:         // Mirror + Flip
                fprintf ( f, "BottomLeft" );
                break;
        }
        fprintf ( f, "</LabelAlignment>\n" );

        fprintf ( f, "    <Priority>%i</Priority>\n", CELL_PRIORITY );
        fprintf ( f, "  </Entity>\n" );

        //
        // Viases
        //

        Coll = GetViasCollection ( Pattern->PatternName );

        if ( Coll )
        {
            Entry = Coll->ViasHead.Flink;

            while ( Entry != &Coll->ViasHead )
            {
                Vias = (PViasEntry)Entry;

                ViasPosX = Vias->OffsetX;
                ViasPosY = Vias->OffsetY;

                switch ( Pattern->Flag & 3 )
                {
                    case 0:         // Normal
                        break;
                    case 1:         // Flipped
                        ViasPosX = CellWidth - ViasPosX;
                        ViasPosY = CellHeight - ViasPosY;
                        break;
                    case 2:         // Mirror
                        ViasPosX = CellWidth - ViasPosX;
                        break;
                    case 3:         // Mirror + Flip
                        ViasPosY = CellHeight - ViasPosY;
                        break;
                }

                ViasPosX += CellPosX;
                ViasPosY += CellPosY;

                fprintf ( f, "  <Entity>\n" );
                fprintf ( f, "    <Label>%s</Label>\n", Vias->ViasName );
                fprintf ( f, "    <LambdaWidth>1</LambdaWidth>\n" );
                fprintf ( f, "    <LambdaHeight>1</LambdaHeight>\n" );
                fprintf ( f, "    <LambdaX>%f</LambdaX>\n", ViasPosX );
                fprintf ( f, "    <LambdaY>%f</LambdaY>\n", ViasPosY );
                fprintf ( f, "    <LambdaEndX>1</LambdaEndX>\n" );
                fprintf ( f, "    <LambdaEndY>1</LambdaEndY>\n" );
                
                fprintf ( f, "    <Type>" );
                switch ( Vias->Type )
                {
                    case ViasInput:
                        fprintf ( f, "ViasInput" );
                        break;
                    case ViasOutput:
                        fprintf ( f, "ViasOutput" );
                        break;
                    case ViasInout:
                        fprintf ( f, "ViasInout" );
                        break;
                }
                fprintf ( f, "</Type>\n" );

                fprintf ( f, "    <ColorOverride>Black</ColorOverride>\n" );
                fprintf ( f, "    <LabelAlignment>TopRight</LabelAlignment>\n" );
                fprintf ( f, "    <Priority>%i</Priority>\n", VIAS_PRIORITY );
                fprintf ( f, "  </Entity>\n" );

                Entry = Entry->Flink;
            }
        }
    }

    fprintf ( f, "</ArrayOfEntity>\n" );

    fclose (f);

    if (Ampersand)
    {
        MessageBox(NULL, "XmlSaver detected Ampersand (&) characters in some of cell names.\n"
                         "Ampersands are restricted by XML standard and replaced to (n)",
                         "Notice",
                         MB_ICONINFORMATION);
    }
}
