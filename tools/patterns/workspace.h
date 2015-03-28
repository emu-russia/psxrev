// Autosave filename.
#define WORKSPACE_AUTOSAVE "Autosave.wrk"

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

    float   Lamda;
    float   LamdaDelta;
    long    Flipped;            // "flip" checkbox state

    //
    // Pattern Database
    //

    long DatabaseOffset;
    long DatabaseLength;

    //
    // Source mathing image layer
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
