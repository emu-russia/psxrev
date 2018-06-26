#pragma once

//
// Workspace image state
// Watch for x64 compatibility! (i.e. not implicit int vatiables, only data types with certain size)
//

typedef struct WorkspaceImage
{
    char Signature[4];          // "WRK\0"

    //
    // Global settings
    //

    float   Lambda;
    float   LambdaDelta;
    long    Flag;               // "flip" / "mirror" checkbox state

    //
    // Pattern Database
    //

	union
	{
		long DatabaseOffset;        // deprecated, replaced by row index
		long RowIndex;
	};
    long DatabaseLength;        // deprecated

    //
    // Source matching image layer
    //

    long SourceImagePresent;
    long SourceImageOffset;
    long SourceImageLength;
    long ScrollX;
    long ScrollY;

    //
    // Added patterns layer
    //

    long PatternsAdded;
    long PatternsLayerOffset;

} WorkspaceImage;

void SaveWorkspace(char *filename);

void LoadWorkspace(char *filename);
