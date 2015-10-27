#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include "jpegwnd.h"
#include "textsaver.h"

static TextPlugin * Plugins = NULL;
static int NumPlugins;
static int SelectedPlugin = -1;

int TextsGetPluginNum(void)
{
    return NumPlugins;
}

TextPlugin * TextsGetPlugin(int Index)
{
    return &Plugins[Index];
}

void TextsAddPlugin(char *Name, TEXT_SAVER Saver)
{
    TextPlugin Plugin;

    strcpy(Plugin.Name, Name);
    Plugin.Saver = Saver;

    Plugins = (TextPlugin *)realloc(Plugins, sizeof(TextPlugin)* (NumPlugins + 1));
    Plugins[NumPlugins++] = Plugin;
}

void TextsSelectPlugin(int Index)
{
    if (Index < 0 || Index >= NumPlugins)
    {
        SelectedPlugin = -1;
        return;
    }

    SelectedPlugin = Index;
}

void TextSave(char *Filename)
{
    PatternEntry * List;
    PatternEntry * Temp;
    int n, Count;

    if (SelectedPlugin != -1)
    {
        //
        // Copy out added pattern entries
        //

        Count = GetPatternEntryNum();

        List = (PatternEntry *)malloc(sizeof(PatternEntry) * Count);

        for (n = 0; n < Count; n++)
        {
            Temp = GetPatternEntry(n);
            memcpy(&List[n], Temp, sizeof(PatternEntry));
        }

        Plugins[SelectedPlugin].Saver(List, Count, Filename);

        free(List);
    }
}
