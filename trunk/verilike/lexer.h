#pragma once

// Типы токенов
enum {
    TOKEN_NULL = 0,
    TOKEN_OP,
    TOKEN_IDENT,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
};

// Операции
enum {
    OP_NOP = 0,
    OP_EQ, OP_POST_EQ,            // =, <=
    OP_LBRACKET, OP_RBRACKET,     // { }
    OP_LPAREN, OP_RPAREN,         // ( )
    OP_LSQAURE, OP_RSQUARE,       // [ ]
    OP_SEMICOLON, OP_COMMA,       // ; ,
};

// Ключевые слова
enum {
    KEYWORD_MODULE = 0,
    KEYWORD_ENDMODULE,
    KEYWORD_BLOCK,
    KEYWORD_BEGIN,
    KEYWORD_END,
    KEYWORD_REG,
    KEYWORD_WIRE,
    KEYWORD_INPUT,
    KEYWORD_OUTPUT,
};

typedef struct token_t 
{
    int     type;
    char    string[256];
    var_t   *var;               // Если токен - идентификатор, тут содержится ссылка на контекст
    var_t   num;                // Если токен - число, тут содержится значение
    int     op;                 // Если токен - операция, тут содержится тип операции
    int     keyword;            // Если токен - ключевое слово, тут содержится его тип
} token_t;

// API.

void tokenize_file ( unsigned char * content, int filesize );
void push_parser ( void (*parser)(token_t * token) );
void pop_parser (void);
token_t * next_token (void);
token_t * prev_token (void);
void feed_token (token_t * token);

