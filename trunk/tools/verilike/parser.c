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
    // раунд < 100 - левая часть, >= 100 - правая часть
    static int round = 0;    // 0: начинаем парсить левую часть, ожидаем { или идентификатор
                             // 1: встретился идентификатор, ожидаем присвоение или [
                             // 2: встретился [, начинаем парсить номер разряда, ожидаем число
                             // 3: встретилось число, ожидаем ]
                             // 4: встретилась ] или }, заканчиваем парсить левую часть, ожидаем присвоение и на 100

                             // 10: встетился {, начинаем парсить список, ожидаем идентификатор
                             // 11: встретился идентификатор или ], ожидаем [, запятую[15] или }[4]
                             // 12: встретился [, ожидаем число
                             // 13: встретилось число, ожидаем ]

                             // 100: встретилось присвоение (= или <=), начинаем парсить правую часть

                             // 200: встретился ;, pop parser.

    static ref_t lvalue[100];
    static int l_num = 0;

    if (token->type == TOKEN_NULL) return;

//    printf ( "round:%i ", round );
//    dummy_parser (token);

    // ЛЕВАЯ ЧАСТь  -- одиночная переменная

    if (round == 0) {
        if (token->type == TOKEN_OP && token->op == OP_LBRACKET) { round = 10; return; }
        else if (token->type == TOKEN_IDENT) {
            strcpy (lvalue[l_num].name, token->string);
            round = 1; return;
        }
    }
    else if (round == 1) {
        if ( token->type == TOKEN_OP ) {
            if (token->op == OP_LSQUARE) { round = 2; return; }
            else if (token->op == OP_EQ || token->op == OP_POSTEQ) {
                lvalue[l_num++].bit = 0;
                round = 100; return; }
        }
    }
    else if (round == 2) {
        if ( token->type == TOKEN_NUMBER ) {
            lvalue[l_num].bit = strtoul ( token->string, 0, 0 );
            round = 3; return;
        }
    }
    else if (round == 3) {
        if ( token->type == TOKEN_OP && token->op == OP_RSQUARE ) {
            round = 4; return;
        }
    }
    else if (round == 4) {
        if ( token->type == TOKEN_OP && (token->op == OP_EQ || token->op == OP_POSTEQ) ) {
            l_num++;
            round = 100; return;
        }
    }

    // ЛЕВАЯ ЧАСТь  -- список переменных {...}

    else if ( round == 10 ) {
        if ( token->type == TOKEN_IDENT) {
            strcpy (lvalue[l_num].name, token->string);
            round = 11; return;
        }
    }
    else if ( round == 11 ) {
        if ( token->type == TOKEN_OP) {
            if ( token->op == OP_LSQUARE) { round = 12; return; }
            else if ( token->op == OP_COMMA) { l_num++; round = 10; return; }
            else if ( token->op == OP_RBRACKET) { round = 4; return; }
        }
    }
    else if ( round == 12 ) {
        if ( token->type = TOKEN_NUMBER ) {
            lvalue[l_num].bit = strtoul ( token->string, 0, 0 );
            round = 13; return;
        }
    }
    else if ( round == 13 ) {
        if ( token->type == TOKEN_OP && token->op == OP_RSQUARE ) {
            round = 11; return;
        }
    }

    // ПРАВАЯ ЧАСТь

    else if (round == 100) {
        // выведем левую часть
        for (int i=0; i<l_num; i++) {
            printf ("%i: %s [%i]\n", i, lvalue[i].name, lvalue[i].bit );
        }
        round++;
    }

//    dummy_parser (token);
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
