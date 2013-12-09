#pragma once

typedef struct node_struct_t {
    struct node_struct_t  *prev;
    struct node_struct_t  *next;        // NULL-терминатор

    ref_t   **inputs;
    int     i_num;      // количество входов

    ref_t   **outputs;
    int     o_num;      // количество выходов

    int     cell_id;

    int     depth;
} node_t;

typedef struct module_t {
    char    name[256];

    node_t  **blocks;       // блоки модуля, NULL-терминатор
    var_t   **reactivity;    // реактивность блоков, NULL-терминатор
    var_t   *reactvals;     // на какие значения блок должен реагировать ('*' - на любые изменения)

} module_t;

typedef struct cell_t {
    char    name[256];
    void    (*cell) (ref_t *inputs, ref_t *outputs);
    int     input_num, output_num;
} cell_t;

extern  char VM_FILE[256];
extern  int  VM_LINE;
extern  int  VM_TERMINATE;

extern  module_t modules[1000];
extern  int num_modules;

extern  cell_t cells[1000];
extern  int num_cells;

void error (char *fmt, ...);
void warning (char *fmt, ...);
int vm_load (char *filename);
int vm_init (void);
void vm_shutdown (void);
void add_cell (char *name, void (*cell)(ref_t *inputs, ref_t *outputs), int i_num, int o_num);
void vm_run (void);
int vm_status (void);

