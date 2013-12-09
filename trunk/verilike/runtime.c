#include "verilike.h"

char VM_FILE[256] = "Not yet";
int  VM_LINE = 0;
int  VM_TERMINATE = 0;

static int vm_initdone = 0;

module_t modules[1000];
int num_modules;

cell_t cells[1000];
int num_cells;

void error (char *fmt, ...)
{
    va_list arg;
    char buf[0x1000];
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    printf ( "ERROR (%s, line %i): %s\n", VM_FILE, VM_LINE, buf );
    VM_TERMINATE = 1;
}

void warning (char *fmt, ...)
{
    va_list arg;
    char buf[0x1000];
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    printf ( "WARNING (%s, line %i): %s\n", VM_FILE, VM_LINE, buf );
}

int vm_load (char *filename)
{
    token_t * token;
    int filesize;
    u8 * content;
    FILE * f = fopen (filename, "rb");  // откроем файл
    if (!f) {
        error ( "Cannot open input file : %s", filename);
        return 0;
    }
    fseek (f, 0, SEEK_END);      // получим размер
    filesize = ftell (f);
    fseek (f, 0, SEEK_SET);

    content = (u8 *)malloc (filesize + 1);      // загрузим   
    if (!content) {
        error ( "Cannot load input file : %s", filename);
        fclose (f);
        return 0;
    }
    fread ( content, 1, filesize, f );
    content[filesize] = 0;  // extra 0 for debug output
    fclose (f);

    strncpy ( VM_FILE, filename, 255 );
    VM_LINE = 1;

    tokenize_file ( content, filesize);
    push_parser ( root_parser );

    do {    // запустить лексический анализ.
        token = next_token ();
        if (token) feed_token ( token );
    } while (token);
    
    //dump_modules ();

    return 1;
}

int vm_init (void)
{
    if ( vm_initdone ) return 1;

    VM_TERMINATE = 0;   // если эта переменная выставляется в 1, то значит нужно прервать процесс компиляции или трансляции.

    vm_initdone = 1;
    return 1;
}

void vm_shutdown (void)
{
    if (vm_initdone) {

        num_modules = num_cells = 0;
        varnum = 0;

        vm_initdone = 0;
    }
}

void add_cell (char *name, void (*cell)(ref_t *inputs, ref_t *outputs), int i_num, int o_num)
{
}

void vm_run (void)
{
}

int vm_status (void)        // 1 - running
{
}
