/*++

Copyright (c)

Module Name:

    objdefs.c

Abstract:

    Определения формата объектных файлов.

--*/

#pragma once

#define OBJ_VERSION 2

//
// Сигнатура
//

#define OBJ_SIGNATURE "LNK"

#define OBJ_SIGNATURE_SIZE 3

//
// Заголовок
//

typedef struct _OBJ_HEADER
{
    char Signature[3];
    char Version;
} OBJ_HEADER, *POBJ_HEADER;

//
// Токены
//

#define OBJ_TOK_CODE 2

