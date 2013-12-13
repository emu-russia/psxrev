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

static int allowed_char (char c, char *allowed)
{
    char *ptr = allowed;
    while (*ptr) {
        if ( *ptr == c ) return 1;
        ptr++;
    }
    return 0;
}

void convertnum ( char *string, int base, token_t * token )
{
    char *ptr = string, ch, * allowed, temp[64];
    int len = strlen (string), bit = 31, i;
    unsigned long hex;

    memset ( token->num, '0', 32 );
    token->num[32] = 0;

    if (base == 2) {
        while (--len >= 0) {
            ch = string[len];
            if ( isdigit (ch) ) token->num[bit--] = ch;
            else if ( ch == 'x' || ch == 'X' || ch == '?' ) token->num[bit--] = 'x';
            else if ( ch == 'z' || ch == 'Z' ) token->num[bit--] = 'z';
            else if ( ch == '/' ) token->num[bit--] = '/';
            else if ( ch == '\\' ) token->num[bit--] = '\\';
        }
    }

    else if (base == 8) {
        allowed = "01234567xXzZ?/\\_";
        while (--len >= 0) {
            ch = string[len];
            if ( allowed_char (ch, allowed) ) {
                switch (ch)
                {
                    case '0':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '1':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '2':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '3':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '4':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case '5':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case '6':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;
                    case '7':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;
                    case 'x':
                    case 'X':
                    case '?':
                        token->num[bit--] = 'x';
                        token->num[bit--] = 'x';
                        token->num[bit--] = 'x';
                        break;
                    case 'z':
                    case 'Z':
                        token->num[bit--] = 'z';
                        token->num[bit--] = 'z';
                        token->num[bit--] = 'z';
                        break;
                    case '/':
                        token->num[bit--] = '/';
                        token->num[bit--] = '/';
                        token->num[bit--] = '/';
                        break;
                    case '\\':
                        token->num[bit--] = '\\';
                        token->num[bit--] = '\\';
                        token->num[bit--] = '\\';
                        break;
                }
            }
        }
    }

    else if (base == 16) {
        allowed = "0123456789abcdefABCDEFxXzZ?/\\_";
        while (--len >= 0) {
            ch = string[len];
            if ( allowed_char (ch, allowed) ) {
                switch (ch)
                {
                    case '0':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '1':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '2':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '3':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        break;
                    case '4':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '5':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '6':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '7':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        break;
                    case '8':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case '9':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case 'a':
                    case 'A':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case 'b':
                    case 'B':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        break;
                    case 'c':
                    case 'C':
                        token->num[bit--] = '0';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;
                    case 'd':
                    case 'D':
                        token->num[bit--] = '1';
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;
                    case 'e':
                    case 'E':
                        token->num[bit--] = '0';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;
                    case 'f':
                    case 'F':
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        token->num[bit--] = '1';
                        break;

                    case 'x':
                    case 'X':
                    case '?':
                        token->num[bit--] = 'x';
                        token->num[bit--] = 'x';
                        token->num[bit--] = 'x';
                        token->num[bit--] = 'x';
                        break;
                    case 'z':
                    case 'Z':
                        token->num[bit--] = 'z';
                        token->num[bit--] = 'z';
                        token->num[bit--] = 'z';
                        token->num[bit--] = 'z';
                        break;
                    case '/':
                        token->num[bit--] = '/';
                        token->num[bit--] = '/';
                        token->num[bit--] = '/';
                        token->num[bit--] = '/';
                        break;
                    case '\\':
                        token->num[bit--] = '\\';
                        token->num[bit--] = '\\';
                        token->num[bit--] = '\\';
                        token->num[bit--] = '\\';
                        break;
                }
            }
        }
    }

    else if (base == 10) {
        hex = strtoul ( string, 0, 10);
        sprintf ( temp, "%08X", hex );
        convertnum ( temp, 16, token );
    }

//    printf ( "Number : %s\n", token->num );
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

static void setop (int op)
{
    current_token.type = TOKEN_OP;
    current_token.op = op;
    strcpy ( current_token.string, opstr(op) );
}

static void setopback (int op)
{
    putback ();
    current_token.type = TOKEN_OP;
    current_token.op = op;
    strcpy ( current_token.string, opstr(op) );
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

    // пропускаем комментарии
    if (ch == '/') {
        ch = nextch (&empty);
        if (empty || (ch != '/' && ch != '*') ) {    // если дальше ничего нет или второй символ не / и не * - вернуть
            if (!empty) putback ();   // вернуть если не пусто
        }
        else if (ch == '*') {   // пропустим все символы до комбинации */  или до конца файла
            while (!empty) {
                ch = nextch (&empty);
                if (empty) break;
                if (ch == '*') {
                    ch = nextch (&empty);
                    if (empty || ch == '/') break;
                    else if (ch == '\n') VM_LINE++;
                }
                else if (ch == '\n') VM_LINE++;
            }
            if (empty) return NULL;
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
        if (isdigit(ch))
        {
            ptr = number;
            *ptr++ = ch;   // если за цифрой ничего нет - то вернуть цифру как 10-ное число
            base = 10;

            // выбрать число в указанной сс
            ch = nextch (&empty);
            if (!empty) {   // выбрать знаки за цифрой
                if ( ch == 'b' || ch == 'B' ) base = 2;
                else if ( ch == 'o' || ch == 'O' ) base = 8;
                else if ( ch == 'x' || ch == 'X' ) base = 16;
                else if ( !isdigit(ch) ) putback ();

                if (base == 2) allowed = "0123456789xXzZ?/\\_";
                else if (base == 8) allowed = "01234567xXzZ?/\\_";
                else if (base == 10) allowed = "0123456789_";
                else if (base == 16) allowed = "0123456789abcdefABCDEFxXzZ?/\\_";

                // выбрать цифры с указанной системой счисления.
                if (base != 10) ch = nextch (&empty);
                if ( allowed_char(ch, allowed) && !empty ) {
                    if (base != 10) ptr = number;
                    *ptr++ = ch;
                    while ( !empty && allowed_char(ch, allowed) ) {
                        ch = nextch (&empty);
                        if (ch != '_' && allowed_char(ch, allowed) ) *ptr++ = ch;
                    }
                    if (!empty) putback ();
                }
            }
            current_token.type = TOKEN_NUMBER;
            *ptr++ = 0;
            if (base == 2) sprintf (current_token.string, "0b%s", number);
            else if (base == 8) sprintf (current_token.string, "0o%s", number);
            else if (base == 16) sprintf (current_token.string, "0x%s", number);
            else if (base == 10) sprintf (current_token.string, "%s", number);
        }

        // конвертировать строку в двоичное представление
        if (current_token.type == TOKEN_NUMBER) convertnum ( number, base, &current_token );
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

