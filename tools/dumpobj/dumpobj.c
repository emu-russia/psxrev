/*++

Copyright (c)

Module Name:

    dumpobj.c

Abstract:

    Консольная программа для дампа структуры объектных файлов.

--*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objdefs.h"

//
// Работа с файлами (порядок байт Little-endian)
//

static unsigned long ReadDword(FILE *f)
{
    unsigned long Result = 0;
    
    Result |= fgetc (f);
    Result |= fgetc (f) << 8;
    Result |= fgetc (f) << 16;
    Result |= fgetc (f) << 24;
    
    return Result;
}

static unsigned short ReadWord(FILE *f)
{
    unsigned short Result = 0;
    
    Result |= fgetc (f);
    Result |= fgetc (f) << 8;
   
    return Result;    
}

static void ReadCode(FILE *f, int Size)
{
    int Num = 0;

    while (Num < Size)
    {
        if ((Num & 0xF) == 0)
            printf("\n%04x:", Num);

        printf("%02x ", fgetc(f));
        Num++;
    }

    printf("\n\n");
}

static void ReadByteString(FILE *f)
{
    int Count = fgetc(f);

    while (Count--)
    {
        fputc(fgetc(f), stdout);
    }
}

static void DumpPatch(FILE *f)
{
    int Byte1;

    Byte1 = fgetc(f);

    if (Byte1 <= 24)
    {
        switch (Byte1)
        {
            case 0:
                printf("$%lx", ReadDword(f));
                return;
            case 2:
                printf("[%lx]", ReadWord(f));
                return;
            case 4:
                printf("sectbase(%lx)", ReadWord(f));
                return;
            case 6:
                printf("bank(%lx)", ReadWord(f));
                return;
            case 8:
                printf("sectof(%lx)", ReadWord(f));
                return;
            case 10:
                printf("offs(%lx)", ReadWord(f));
                return;
            case 12:
                printf("sectstart(%lx)", ReadWord(f));
                return;
            case 14:
                printf("groupstart(%lx)", ReadWord(f));
                return;
            case 16:
                printf("groupof(%lx)", ReadWord(f));
                return;
            case 18:
                printf("seg(%lx)", ReadWord(f));
                return;
            case 20:
                printf("grouporg(%lx)", ReadWord(f));
                return;
            case 22:
                printf("sectend(%lx)", ReadWord(f));
                return;
            case 24:
                printf("groupend(%lx)", ReadWord(f));
                return;
        }
    }

    printf("(");
    DumpPatch(f);

    switch (Byte1)
    {
        case 32: printf("="); break;
        case 34: printf("<>"); break;
        case 36: printf("<="); break;
        case 38: printf("<"); break;
        case 40: printf(">="); break;
        case 42: printf(">"); break;
        case 44: printf("+"); break;
        case 46: printf("-"); break;
        case 48: printf("*"); break;
        case 50: printf("/"); break;
        case 52: printf("&"); break;
        case 54: printf("!"); break;
        case 56: printf("^"); break;
        case 58: printf("<<"); break;
        case 60: printf(">>"); break;
        case 62: printf("%%"); break;
        case 64: printf("---"); break;
        case 66: printf("-revword-"); break;
        case 68: printf("-check0-"); break;
        case 70: printf("-check1-"); break;
        case 72: printf("-bitrange-"); break;
        case 74: printf("-arshift_chk-"); break;

        default:
            printf("?%d?", Byte1);
            break;
    }

    DumpPatch(f);
    printf(")");
}

static int DumpObjInfo(FILE *f, int ShowCode)
{
    int Token;
    int BreakStream = 0;
    int Result = 0;
    unsigned short Word1, Word2;
    unsigned char Byte;
    unsigned long Dword1;

    while (1)
    {
        Token = fgetc(f);
        if (Token < 0 || BreakStream)
            break;

        switch (Token)
        {
            case 0:
                printf("0 : End of file\n");
                break;

            case 2:
                Word1 = ReadWord (f);
                printf("2 : Code %ld bytes\n", Word1);
                
                if ( ShowCode )
                    ReadCode(f, Word1);
                else
                    fseek ( f, Word1, SEEK_CUR );
                break;

            case 4:

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);

                printf("4 : Run at offset %lx in section %lx\n", Dword1, Word1);

                break;

            case 6:

                printf("6 : Switch to section %lx\n", ReadWord(f));

                break;

            case 8:

                printf("8 : Uninitialised data, %ld bytes\n", ReadDword(f));

                break;

            case 10:

                Byte = fgetc(f);
                Word1 = ReadWord(f);

                printf("10 : Patch type %d at offset %lx with ", Byte, Word1);
                DumpPatch(f);
                printf("\n");

                break;

            case 12:

                Word1 = ReadWord(f);
                Word2 = ReadWord(f);
                Dword1 = ReadDword(f);
                
                printf("12 : XDEF symbol number %lx \'", Word1);
                ReadByteString(f);
                printf("\' at offset %lx in section %lx\n", Dword1, Word2);

                break;

            case 14:

                printf("14 : XREF symbol number %lx \'", ReadWord(f));
                ReadByteString(f);
                printf("\'\n");

                break;

            case 16:

                Word1 = ReadWord(f);
                Word2 = ReadWord(f);
                Byte = fgetc(f);

                printf("16 : Section symbol number %lx \'", Word1);
                ReadByteString(f);
                printf("\' in group %lx alignment %d\n", Word2, Byte );

                break;

            case 18:

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);

                printf("18 : Local symbol \'");
                ReadByteString(f);
                printf("\' at offset %lx in section %lx\n", Dword1, Word1);

                break;

            case 20:

                Word1 = ReadWord(f);
                Byte = fgetc(f);

                printf("20 : Group symbol number %lx \'", Word1);
                ReadByteString(f);
                printf("\' type %d\n", Byte);

                break;

            case 22:

                printf("22 : Set byte size register to ");
                DumpPatch(f);
                printf("  reg offset %d\n", ReadWord(f));

                break;

            case 24:

                printf("24 : Set word size register to ");
                DumpPatch(f);
                printf("  reg offset %d\n", ReadWord(f));

                break;

            case 26:

                printf("26 : Set long size register to ");
                DumpPatch(f);
                printf("  reg offset %d\n", ReadWord(f));

                break;

            case 28:

                printf("28 : Define file ");
                printf("number %x as \"", ReadWord(f));
                ReadByteString(f);
                printf("\"\n");

                break;

            case 30:

                printf("30 : Set to ");

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);

                printf("file %lx, line %ld\n", Word1, Dword1);

                break;

            case 32:

                printf("32 : Set to ");
                printf("line %ld\n", ReadDword(f));

                break;

            case 34:

                printf("34 : Increment line number\n");

                break;

            case 36:

                printf("36 : Increment line number by ");
                printf("%d\n", fgetc(f) & 0xFF );

                break;

            case 38:

                printf("38 : Increment line number by ");
                printf("%d\n", ReadWord(f));

                break;

            case 40:

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);

                printf("40 : Very local symbol \'");
                ReadByteString(f);
                printf("\' at offset %lx in section %lx\n", Dword1, Word1);

                break;

            case 42:

                printf("42 : Set 3-byte size register to ");
                DumpPatch(f);
                printf("  reg offset %d\n", ReadWord(f));

                break;

            case 44:

                Byte = fgetc(f);
                Word1 = ReadWord(f);
                printf("44 : Set MX info at offset %x to %x\n", Word1, Byte);

                break;

            case 46:
                printf("46 : Processor type %d\n", fgetc(f) & 0xFF);
                break;

            case 48:
                
                Word1 = ReadWord(f);
                Word2 = ReadWord(f);
                Dword1 = ReadDword(f);

                printf("48 : XBSS symbol number %lx \'", Word1);
                ReadByteString(f);
                printf("\' size %lx in section %lx\n", Dword1, Word2 );

                break;

            case 50:

                printf("50 : Inc SLD linenum at offset %lx\n", ReadWord(f));

                break;

            case 52:

                Word1 = ReadWord(f);
                Byte = fgetc(f);
                printf("52 : Inc SLD linenum by byte %u at offset %lx\n", Byte, Word1);

                break;

            case 54:

                Word1 = ReadWord(f);
                Word2 = ReadWord(f);
                printf("54 : Inc SLD linenum by word %lu at offset %lx\n", Word2, Word1);

                break;

            case 56:

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);
                printf("56 : Set SLD linenum to %lu at offset %lx\n", Dword1, Word1);

                break;

            case 58:

                Word1 = ReadWord(f);
                Dword1 = ReadDword(f);
                Word2 = ReadWord(f);
                printf("58 : Set SLD linenum to %lu at offset %lx in file %lx\n", Dword1, Word1, Word2);

                break;

            case 60:

                printf("60 : End SLD info at offset %lx\n", ReadWord(f));

                break;

            case 62:

                printf("62 : Repeat byte ");
                DumpPatch(f);
                printf(" %ld times\n", ReadDword(f));

                break;

            case 64:

                printf("64 : Repeat word ");
                DumpPatch(f);
                printf(" %ld times\n", ReadDword(f));

                break;

            case 66:

                printf("66 : Repeat longword ");
                DumpPatch(f);
                printf(" %ld times\n", ReadDword(f));

                break;

            case 68:

                printf("68 : Proc call ");

                Byte = fgetc(f);
                if (Byte)
                    printf("FAR ");
                else
                    printf("NEAR ");
                
                printf("to symbol %lx ", ReadWord(f));

                Byte = fgetc(f);
                Word1 = ReadWord(f);

                switch (Byte)
                {
                    case 0:
                        printf("dp:? ");
                        break;
                    case 2:
                        printf("dp:$%lx ", Word1);
                        break;
                    case 4:
                        printf("dp:(section %lx) ", Word1);
                        break;
                    case 6:
                        printf("dp:(group %lx) ", Word1);
                        break;
                    default:
                        printf("dp:?%x/%lx? ", Byte, Word1);
                        break;
                }

                Byte = fgetc(f);
                Word1 = ReadWord(f);

                switch (Byte)
                {
                    case 0:
                        printf("db:? ");
                        break;
                    case 2:
                        printf("db:$%lx ", Word1);
                        break;
                    case 4:
                        printf("db:(section %lx) ", Word1);
                        break;
                    case 6:
                        printf("db:(group %lx) ", Word1);
                        break;
                    default:
                        printf("db:?%x/%lx? ", Byte, Word1);
                        break;
                }

                Byte = fgetc(f);
                if (Byte)
                    printf("A:long ");
                else
                    printf("A:short ");

                Byte = fgetc(f);
                if (Byte)
                    printf("I:long\n");
                else
                    printf("I:short\n");

                break;

            case 70:

                printf("68 : Proc definition ");
                printf("symbol %lx ", ReadWord(f));

                Byte = fgetc(f);
                Word1 = ReadWord(f);

                switch (Byte)
                {
                    case 0:
                        printf("dp:? ");
                        break;
                    case 2:
                        printf("dp:$%lx ", Word1);
                        break;
                    case 4:
                        printf("dp:(section %lx) ", Word1);
                        break;
                    case 6:
                        printf("dp:(group %lx) ", Word1);
                        break;
                    default:
                        printf("dp:?%x/%lx? ", Byte, Word1);
                        break;
                }

                Byte = fgetc(f);
                Word1 = ReadWord(f);

                switch (Byte)
                {
                    case 0:
                        printf("db:? ");
                        break;
                    case 2:
                        printf("db:$%lx ", Word1);
                        break;
                    case 4:
                        printf("db:(section %lx) ", Word1);
                        break;
                    case 6:
                        printf("db:(group %lx) ", Word1);
                        break;
                    default:
                        printf("db:?%x/%lx? ", Byte, Word1);
                        break;
                }

                Byte = fgetc(f);

                if (Byte != EOF)
                    printf("MX:%x ", Byte);

                Byte = fgetc(f);

                if (Byte != EOF)
                {
                    if (Byte)
                        printf("FAR");
                    else
                        printf("NEAR");
                }

                printf("\n");

                break;

            case 72:

                printf("72 : Repeat 3-byte ");
                DumpPatch(f);
                printf(" %ld times\n", ReadDword(f));

                break;

            case 74:

                printf("74 : Function start :\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  offset $%08lx\n", ReadDword(f));
                printf("  file %04x\n", ReadWord(f));
                printf("  start line %ld\n", ReadDword(f));
                printf("  frame reg %d\n", ReadWord(f));
                printf("  frame size %ld\n", ReadDword(f));
                printf("  return pc reg %d\n", ReadWord(f));
                printf("  mask $%08lx\n", ReadDword(f));
                printf("  mask offset %ld\n", ReadDword(f));
                printf("  name ");
                ReadByteString(f);
                printf("\n");

                break;

            case 76:

                printf("76 : Function end :\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  offset $%08lx\n", ReadDword(f));
                printf("  end line %ld\n", ReadDword(f));

                break;

            case 78:

                printf("78 : Block start :\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  offset $%08lx\n", ReadDword(f));
                printf("  start line %ld\n", ReadDword(f));

                break;

            case 80:

                printf("80 : Block end\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  offset $%08lx\n", ReadDword(f));
                printf("  end line %ld\n", ReadDword(f));

                break;

            case 82:

                printf("82 : Def :\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  value $%08lx\n", ReadDword(f));
                printf("  class %d\n", ReadWord(f));
                printf("  type %d\n", ReadWord(f));
                printf("  size %ld\n", ReadDword(f));
                printf("  name : ");
                ReadByteString(f);
                printf("\n");

                break;

            case 84:

                printf("84 : Def2 :\n");
                printf("  section %04x\n", ReadWord(f));
                printf("  value $%08lx\n", ReadDword(f));
                printf("  class %d\n", ReadWord(f));
                printf("  type %d\n", ReadWord(f));
                printf("  size %ld\n", ReadDword(f));
                
                Word1 = ReadWord(f);
                printf("  dims %ld ", Word1);

                while (Word1--)
                {
                    printf("%ld ", ReadDword(f));
                }

                printf("\n  tag ");
                ReadByteString(f);
                printf("\n");
                ReadByteString(f);
                printf("\n");

                break;

            default:
                printf ( "%d : Unknown tag - exiting\n", Token );
                BreakStream = 1;
                break;
        }
    }

    return Result;
}

int main(int argc, char **argv)
{
    FILE *f;
    char Sig[OBJ_SIGNATURE_SIZE + 1];
    int ShowCode;
    int i;
    char Version;
    int Result;
    char *filename;

    //
    // Проверить параметры
    //
    // Аргумент может быть /c
    //

    if (argc < 2 || argc > 3)
    {
        printf("Usage: objdump [/c] <obj_file>\n");
        return -1;
    }

    if (argc > 2)
    {
        if ( ! ( argv[1][0] == '/' && (argv[1][1] == 'c' || argv[1][1] == 'C') ) )
        {
            printf("Error : invalid switch %s - exiting\n", argv[1]);
            return -2;
        }

        filename = argv[2];
    }
    else
        filename = argv[1];

    //
    // Открыть файл
    //

    f = fopen(filename, "rb");
    if (!f)
    {
        printf("Error : unable to open file \'%s\' for input - exiting\n", filename);
        return -3;
    }

    if (argc <= 2)
        ShowCode = 0;
    else
        ShowCode = 1;

    //
    // Прочитать сигнатуру 
    //

    memset(Sig, 0, sizeof(Sig));

    for (i = 0; i < OBJ_SIGNATURE_SIZE; i++)
    {
        Sig[i] = fgetc(f);
    }

    //
    // Проверить сигнатуру
    //

    if (strcmp(Sig, OBJ_SIGNATURE) != 0)
    {
        printf("Error : file \'%s\' is not an SNLink format object file - exiting\n", filename);

        fclose(f);
        return -4;
    }

    //
    // Версия
    //

    Version = fgetc(f);

    if (Version != OBJ_VERSION)
    {
        printf( "Error - file \'%s\' is in an %s SNLink object file format - exiting\n",
                filename, Version < OBJ_VERSION ? "obsolete" : "unknown");

        fclose(f);
        return -5;
    }

    printf("\nHeader : version %d\n", Version);

    Result = DumpObjInfo(f, ShowCode);

    fclose(f);
    return Result;
}
