// Швейцарский набор парсеров на все случаи жизни.
#include "verilike.h"

void root_parser (token_t * token)
{
    dummy_parser (token);
}

void module_parser (token_t * token)
{
}

void block_parser (token_t * token)
{
}

void begin_parser (token_t * token)
{
}

void rt_parser (token_t * token)
{
}

void ref_parser (token_t *token)
{
}

// наш отладочный парсер. ничего не делает - просто выводит поток токенов на экран, для диагностики.
void dummy_parser (token_t * token)
{
    if ( token->type != TOKEN_NULL) {
        if (token->type == TOKEN_OP) printf ( "%s, op: %s\n", token_type(token->type), opstr(token->op) );
        else if (token->type == TOKEN_KEYWORD) printf ( "%s, keyword: %i, raw=%s\n", token_type(token->type), token->keyword, token->string );
        //else if (token->type == TOKEN_NUMBER) dumpnum ( &token->num );
        else printf ( "%s, raw=\'%s\'\n", token_type(token->type), token->string );
    }
}
