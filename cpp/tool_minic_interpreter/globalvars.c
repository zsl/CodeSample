#include "typedef.h"

struct var_type global_vars[NUM_GLOBAL_VARS];
struct var_type local_var_stack[NUM_LOCAL_VARS];

struct func_type func_table[NUM_FUNC];
struct func_type func_stack[NUM_FUNC];
int call_stack[NUM_FUNC];

struct commands table[] = {
	"if", IF, /* in this table. */
	"else", ELSE,
	"for", FOR,
	"do", DO,
	"while", WHILE,
	"char", CHAR,
	"int", INT,
	"return", RETURN,
	"end", END,
	"", END  /* mark end of table */
};

struct intern_func_type intern_func[] = {
	"getche", call_getche,
	"putch",  call_putch,
	"puts",   call_puts,
	"print",  print,
	"getnum", getnum,
	"",      0
};

char *prog; /* current location in source code */
char *p_buf;/* point to start of program buffer */
jmp_buf e_buf; /* hold environment for longjmp() */

char tok;   /* 标识是哪个关键字，如for */
char token[80]; /* string representation of token */
char token_type; /* 标识是哪种类型的标记，如KEYWORD */

int ret_value; /* function return value */

int functos;  /* index to top of function call stack */
int func_index; /* index into function table */
int gvar_index; /* index into global variable table */
int lvartos; /* index into local variable stack */