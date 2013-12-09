#pragma once

// Значения по умолчанию
#define WIRE_DEFAULT 'z'
#define REG_DEFAULT 'x'

// Типы переменных контекста
enum {
    VAR_UNKNOWN = 0,
    VAR_WIRE,
    VAR_REG,
    VAR_INPUT = 0x100,      // модификаторы
    VAR_OUTPUT = 0x200,
};

typedef struct var_t
{
    int     type;
    char    name[256];
    int     module_id;
    char    value[32];
    char    old[32];
    char    shadow[32];
    int     bits;
} var_t;

extern  var_t   context[1024];
extern  int     varnum;
