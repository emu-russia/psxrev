// Менеджер плагинов для сохранения паттернов в текстовой файл.

typedef struct TextPlugin
{
    char    Name[0x100];
    void    (*Saver)(PatternEntry *List, int Count);
} TextPlugin;

int     TextsGetPluginNum(void);

void    TextsAddPlugin(char *Name, void(*Saver)(PatternEntry *List, int Count));

void    TextsSelectPlugin(int Index);

void    TextSave(PatternEntry *List, int Count);
