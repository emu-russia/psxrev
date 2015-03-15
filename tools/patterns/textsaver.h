// Pattern layer text saving manager.

typedef void(*TEXT_SAVER)(PatternEntry *List, int Count, char *Filename);

typedef struct TextPlugin
{
    char    Name[0x100];
    TEXT_SAVER Saver;
} TextPlugin;

int     TextsGetPluginNum(void);

TextPlugin * TextsGetPlugin(int Index);

void    TextsAddPlugin(char *Name, TEXT_SAVER Saver);

void    TextsSelectPlugin(int Index);

void    TextSave(char *Filename);
