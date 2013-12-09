// Лексический анализатор Verilike.
#include "verilike.h"

static void (*parser_stack[1024])(token_t * token) ;
static int parser_stack_pointer;
static token_t previous_token, current_token;
static int tokenization_started = 0;
static u8 * token_source;
static int token_source_pointer, token_source_length;

// Ключевые слова.
char * keywords[] = {
    "module", "endmodule", "block", "begin", "end", "reg", "wire", "input", "output", NULL,
};

char * opstr (int type)
{
    char *str[] = {
        "NOP", "=", "<=", "{", "}", "(", ")", "[", "]", ";", ",",
    };
    return str[type];
}

char * token_type (int type)
{
    switch (type)
    {
        case TOKEN_NULL : return "NULL";
        case TOKEN_OP : return "OP";
        case TOKEN_NUMBER : return "NUMBER";
        case TOKEN_IDENT : return "IDENT";
        case TOKEN_KEYWORD : return "KEYWORD";
        default : return "UNKNOWN";
    }
}

void tokenize_file ( unsigned char * content, int filesize )   // подключить загруженный файл 
{
    tokenization_started = 0;
    token_source = content;
    token_source_pointer = 0;
    token_source_length = filesize;
    parser_stack_pointer = 0;
    parser_stack[0] = NULL;
    memset ( &previous_token, 0, sizeof(token_t) );
}

void push_parser ( void (*parser)(token_t * token) )  // подцепить парсер к потоку, старый положить в стек
{
    if ( parser_stack_pointer > 1023 ) error ( "Parser stack is full (too many nested blocks)!" );
    parser_stack[++parser_stack_pointer] = parser;
}

void pop_parser (void)   // вернуть старый парсер из стека
{
    if (parser_stack_pointer > 0) parser_stack_pointer--;
}

static unsigned char nextch (int * empty)   // получить следующий символ
{
    if ( token_source_pointer < token_source_length && !VM_TERMINATE ) {
        *empty = 0;
        return token_source[token_source_pointer++];
    }
    else {
        *empty = 1;
        return 0;
    }
}

static void putback (void)   // положить назад где взяли
{
    if ( token_source_pointer > 0 ) token_source_pointer--;
}

static int allowed_char (char c, char *allowed)
{
    char *ptr = allowed;
    while (*ptr) {
        if ( *ptr == c ) return 1;
        ptr++;
    }
    return 0;
}

static void setop (int op)
{
    current_token.type = TOKEN_OP;
    current_token.op = op;
    strcpy ( current_token.string, opstr[op] );
}

static void setopback (int op)
{
    putback ();
    current_token.type = TOKEN_OP;
    current_token.op = op;
    strcpy ( current_token.string, opstr[op] );
}

token_t * next_token (void)  // получить следующий токен или вернуть NULL, если конец файла
{
    int empty, ident_max_size, number_max_size, base, key;
    token_t * pt = &previous_token;
    u8 ch, size[32], number[256], ident[1024], *ptr;
    char * allowed, ** words;

    // копируем текущий токен в предыдущий, только если он не пустышка.
    if (current_token.type != TOKEN_NULL) memcpy ( &previous_token, &current_token, sizeof(token_t) );

    // ну просто дубовый алгоритм, выбираем символы и смотрим что получается )))

    current_token.type = TOKEN_NULL;
    current_token.string[0] = 0;

    ch = nextch (&empty);
    if ( empty ) return NULL;

    // пропускаем пробелы
    if ( ch <= ' ' ) {
        while (!empty) {
            ch = nextch (&empty);
            if (ch == '\n') VM_LINE++;
            if (ch <= ' ') continue;
            else break;
        }
        if (empty) return NULL;
    }

    // пропускаем однострочные комментарии
    if (ch == '/') {
        ch = nextch (&empty);
        if (empty || ch != '/') {    // если дальше ничего нет или второй символ не / - вернуть
            if (!empty) putback ();   // вернуть если не пусто
        }
        else {   // пропустим все символы до конца строка '\n' или конца файла
            while (!empty) {
                ch = nextch (&empty);
                if (ch == '\n') { VM_LINE++; break; }
            }
            if (empty) return NULL;
        }
    }
    // .... если после пропуска однострочных комментариев в строке остались пробелы или табуляции, то дальнейшее исполнение ни к чему не приведёт и вернется TOKEN_NULL.

    // пропускаем многострочные комментарии (потенциально - деление /) 

    // .... если после пропуска многострочных комментариев в строке остались пробелы или табуляции, то дальнейшее исполнение ни к чему не приведёт и вернется TOKEN_NULL.

    // идентификаторы / ключевые слова
    if ( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_') )
    {
        ident_max_size = 1000;   // установить максимальный размер идентификатора
        ptr = ident;
        *ptr++ = ch;
        while (!empty && ident_max_size) {
            ch = nextch (&empty);
            if ( !isalpha(ch) && !isdigit(ch) && (ch != '_') ) {
                if (!empty) putback ();
                *ptr++ = 0;
                break;
            }
            if (!empty) {
                *ptr++ = ch;
                ident_max_size--;
            }
        }
        if (empty) *ptr++ = 0;
        if (ident_max_size == 0) warning ( "Identifier max. length exceeds limit" );

        // вернуть ключевое слово или идентификатор.
        key = 0;
        words = keywords;
        while (*words) {
            if ( !strcmp ( ident, *words) ) {
                current_token.type = TOKEN_KEYWORD;
                current_token.keyword = key;
                break;
            }
            words++;  key++;
        }
        if (*words == NULL) current_token.type = TOKEN_IDENT;
        strncpy ( current_token.string, ident, 255 );
    }

    // числа
    if (current_token.type == TOKEN_NULL) 
    {
/*
        if (isdigit(ch) || ch == '\'' ) {
            number[0] = 0;
            base = 0;

            // получить размер числа. если за размером не идёт уточняющий префикс, то вернуть размер как 10-чное число.
            allowed = "0123456789_";
            if ( allowed_char(ch, allowed) ) {
                ptr = size;
                *ptr++ = ch;
                while ( !empty && allowed_char(ch, allowed) ) {
                    ch = nextch (&empty);
                    if (ch != '_' && allowed_char(ch, allowed) ) *ptr++ = ch;
                }
                *ptr++ = 0;
            }
            else strcpy (size, "32");

            // пропустить пробелы.
            if ( ch <= ' ' ) {
                while (!empty) {
                    ch = nextch (&empty);
                    if (ch == '\n') VM_LINE++;
                    if (ch <= ' ') continue;
                    else break;
                }
                if (empty) {        // вернуть размер как число.
                    current_token.type = TOKEN_NUMBER;
                }
            }

            // получить основание числа
            if (ch == '\'' && current_token.type == TOKEN_NULL ) {
                ch = nextch (&empty);
                if (!empty) {
                    if ( ch == 'b' || ch == 'B' ) base = 2;
                    else if ( ch == 'o' || ch == 'O' ) base = 8;
                    else if ( ch == 'd' || ch == 'D' ) base = 10;
                    else if ( ch == 'h' || ch == 'H' ) base = 16;
                }
                if (base == 0) warning ("Unknown base : [%c]", ch);
                if (base == 2) allowed = "01xXzZ?_";
                else if (base == 8) allowed = "01234567xXzZ?_";
                else if (base == 10) allowed = "0123456789_";
                else if (base == 16) allowed = "0123456789abcdefABCDEFxXzZ?_";
                ch = nextch (&empty);
            }
            else {      // вернуть размер как число.
                putback ();
                current_token.type = TOKEN_NUMBER;
            }

            // пропустить пробелы.
            if ( ch <= ' ' && current_token.type == TOKEN_NULL ) {
                while (!empty) {
                    ch = nextch (&empty);
                    if (ch == '\n') VM_LINE++;
                    if (ch <= ' ') continue;
                    else break;
                }
                if (empty) {        // вернуть число 0, если за указанием основания числа ничего нет
                    current_token.type = TOKEN_NUMBER;
                    strcpy (size, "0");
                }
            }

            // выбрать цифры с указанной системой счисления.
            if ( allowed_char(ch, allowed) && current_token.type == TOKEN_NULL ) {
                ptr = number;
                *ptr++ = ch;
                while ( !empty && allowed_char(ch, allowed) ) {
                    ch = nextch (&empty);
                    if (ch != '_' && allowed_char(ch, allowed) ) *ptr++ = ch;
                }
                if (!empty) putback ();
                *ptr++ = 0;
                current_token.type = TOKEN_NUMBER;
            }

            // конвертировать число и выполнить популяцию 
            if ( current_token.type == TOKEN_NUMBER ) {
                if (base) convertnum (number, &current_token.num, base, 0, atoi(size) - 1);
                else convertnum (size, &current_token.num, 10, 0, 31);
            }
        }
*/
    }

    // однознаковые операции
    if ( current_token.type == TOKEN_NULL )
    {
        switch (ch)
        {
            case '[': setop (OP_LSQUARE); break;
            case ']': setop (OP_RSQUARE); break;
            case '{': setop (OP_LBRACKET); break;
            case '}': setop (OP_RBRACKET); break;
            case '(': setop (OP_LPAREN); break;
            case ')': setop (OP_RPAREN); break;
            case ',': setop (OP_COMMA); break;
            case ';': setop (OP_SEMICOLON); break;
            case '=': setop (OP_EQ); break;
        }
    }

    // многознаковые операции: '<='
    if ( current_token.type == TOKEN_NULL )
    {
        if ( ch == '<' ) {      // <=
            ch = nextch (&empty);
            if (!empty )
            {
                if ( ch == '=') setop (OP_POSTEQ);
                else putback ();
            }
        }
    }
    // все остальные токены мы просто игнорируем.

    tokenization_started = 1;
    return &current_token;
}

token_t * prev_token (void)  // предыдущий токен (или NULL, если токена не было)
{
    if (!tokenization_started) return NULL;
    else return &previous_token;
}

void feed_token (token_t * token)    // скормить токен подключенному парсеру
{
    if (token && parser_stack[parser_stack_pointer]) parser_stack[parser_stack_pointer] (token);
}

