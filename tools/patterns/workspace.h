// Имя файла для автосохранения.
#define WORKSPACE_AUTOSAVE "Autosave.wrk"

// Образ состояния рабочей среды.
typedef struct WorkspaceImage
{
    char Signature[4];          // "WRK\0"

    //
    // Глобальные переменные
    //

    float   Lamda;
    float   LamdaDelta;
    bool    Flipped;            // Галочка "flip"

    //
    // База паттернов
    //

    long DatabaseOffset;
    long DatabaseLength;

    //
    // Плоскость исходной картинки
    //

    bool SourceImagePresent;
    long SourceImageOffset;
    long SourceImageLength;
    int ScrollX;
    int ScrollY;

    //
    // Плоскость добавленных паттернов
    //

    long PatternsAdded;
    long PatternsLayerOffset;

} WorkspaceImage;

void SaveWorkspace(char *filename);

void LoadWorkspace(char *filename);
