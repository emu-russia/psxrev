// Autosave filename.
#define WORKSPACE_AUTOSAVE "Autosave.wrk"

//
// Workspace image state
//

typedef struct WorkspaceImage
{
    char Signature[4];          // "WRK\0"

    //
    // Global settings
    //

    float   Lamda;
    float   LamdaDelta;
    bool    Flipped;            // "flip" checkbox state

    //
    // Pattern Database
    //

    long DatabaseOffset;
    long DatabaseLength;

    //
    // Source mathing image layer
    //

    bool SourceImagePresent;
    long SourceImageOffset;
    long SourceImageLength;
    int ScrollX;
    int ScrollY;

    //
    // Added patterns layer
    //

    long PatternsAdded;
    long PatternsLayerOffset;

} WorkspaceImage;

void SaveWorkspace(char *filename);

void LoadWorkspace(char *filename);
