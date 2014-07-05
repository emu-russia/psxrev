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
    OP_EQ, OP_POSTEQ,            // =, <=
    OP_LBRACKET, OP_RBRACKET,     // { }
    OP_LPAREN, OP_RPAREN,         // ( )
    OP_LSQUARE, OP_RSQUARE,       // [ ]
    OP_SEMICOLON, OP_COMMA,       // ; ,
};

// Ключевые слова
enum {
    KEYWORD_MODULE = 0,
    KEYWORD_ENDMODULE,
    KEYWORD_BLOCK,
    KEYWORD_BEGIN,
    KEYWORD_END,
    KEYWORD_WIRE,
    KEYWORD_INPUT,
    KEYWORD_OUTPUT,
};

typedef struct token_t 
{
    int     type;
    char    string[256];        // Если токен - идентификатор, тут содержится имя. В остальных случаях тут содержится текст токена.
    char    num[32+1];            // Если токен - число, тут содержится значение,
    int     op;                 // Если токен - операция, тут содержится тип операции
    int     keyword;            // Если токен - ключевое слово, тут содержится его тип
} token_t;

// API.

extern char * keywords[];

char * opstr (int type);
char * token_type (int type);
void tokenize_file ( unsigned char * content, int filesize );
void push_parser ( void (*parser)(token_t * token) );
void pop_parser (void);
token_t * next_token (void);
token_t * prev_token (void);
void feed_token (token_t * token);

