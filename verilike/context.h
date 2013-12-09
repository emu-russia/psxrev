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

typedef struct var_t        // контекст переменной
{
    int     type;
    char    name[256];
    int     module_id;
    char    value[32];
    char    old[32];
    char    shadow[32];
    int     bits;
} var_t;

typedef struct ref_t        // ссылка на разряд переменной
{
    var_t   *ptr;
    int     bit;
} ref_t;

extern  var_t   context[1024];
extern  int     varnum;

void    set_context ( char *varname, var_t * value );
var_t   get_context ( char *varname );
