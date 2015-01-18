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

void TextsAddPlugin(char *Name, void(*Saver)(PatternEntry *List, int Count))
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

void TextSave(PatternEntry *List, int Count)
{
    if (SelectedPlugin != -1)
    {
        Plugins[SelectedPlugin].Saver(List, Count);
    }
}
