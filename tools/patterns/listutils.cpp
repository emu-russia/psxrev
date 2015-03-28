// Convenient double-linked list utils from Ntos core.

#include <Windows.h>

void InitializeListHead(PLIST_ENTRY ListHead)
{
    ListHead->Blink = ListHead->Flink = ListHead;
}

void InsertTailList(PLIST_ENTRY ListHead, PLIST_ENTRY Entry)
{
    PLIST_ENTRY OldBlink;
    OldBlink = ListHead->Blink;
    Entry->Flink = ListHead;
    Entry->Blink = OldBlink;
    OldBlink->Flink = Entry;
    ListHead->Blink = Entry;
}

void RemoveEntryList(PLIST_ENTRY Entry)
{
    PLIST_ENTRY OldFlink;
    PLIST_ENTRY OldBlink;

    OldFlink = Entry->Flink;
    OldBlink = Entry->Blink;
    OldFlink->Blink = OldBlink;
    OldBlink->Flink = OldFlink;
}

BOOLEAN IsListEmpty(PLIST_ENTRY ListHead)
{
    return ListHead->Flink == ListHead;
}
