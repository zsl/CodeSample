#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <setjmp.h>

#define NUM_FUNC        100
#define NUM_GLOBAL_VARS 100
#define NUM_LOCAL_VARS  100
#define NUM_BLOCK       100
#define ID_LEN          31
#define FUNC_CALLS      31
#define NUM_PARAMS      31
#define PROG_SIZE       10000
#define FOR_NEST        31
#define LOOP_NEST       31

enum tok_types{
	DELIMITER,
	IDENTIFIER,
	NUMBER,
	KEYWORD,
	TEMP,
	STRING,
	BLOCK
};

/* add additional C keyword tokens here */
enum tokens {ARG, CHAR, INT, IF, ELSE, FOR, DO, WHILE,
             SWITCH, RETURN, EOL, FINISHED, END};

enum double_pos{
	LT = 1,
	LE,
	GT,
	GE,
	EQ,
	NE,
};

enum error_msg
     {SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED,
      NOT_VAR, PARAM_ERR, SEMI_EXPECTED,
      UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED,
      NEST_FUNC, RET_NOCALL, PAREN_EXPECTED,SEMI_EXPECPED,
      WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP,
      TOO_MANY_LVARS, DIV_BY_ZERO};

struct var_type{
	char var_name[32];
	int v_type;
	int value;
};
extern struct var_type global_vars[];
extern struct var_type local_var_stack[];

struct func_type{
	char func_name[32];
	int ret_type;
	char *loc;  // location of function entry point in file
};

extern struct func_type func_table[];
extern struct func_type func_stack[];

extern int call_stack[];

typedef struct commands{
	char command[20];
	char tok;
}commands;
extern struct commands table[];

struct intern_func_type{
	char *f_name;  // function name
	int (*p)();    // pointer to the function
}intern_func[];


extern int call_getche(void), call_putch(void);
extern int call_puts(void), print(void), getnum(void);


extern char *prog; /* current location in source code */
extern char *p_buf;/* point to start of program buffer */
extern jmp_buf e_buf; /* hold environment for longjmp() */

extern char tok;   /* internal representation of token */
char token[80]; /* string representation of token */
extern char token_type; /* contains type of token */

extern int ret_value; /* function return value */

extern int functos;  /* index to top of function call stack */
extern int func_index; /* index into function table */
extern int gvar_index; /* index into global variable table */
extern int lvartos; /* index into local variable stack */

extern int iswhite(char c);
extern int isdelim(char c);
extern int look_up(char *s);
extern void sntx_err(int error);
extern void eval_exp0(int *value);
extern void eval_exp(int *value);
extern void eval_exp1(int *value);
extern void eval_exp2(int *value);
extern void eval_exp3(int *value);
extern void eval_exp4(int *value);
extern void eval_exp5(int *value);
extern void atom(int *value);
extern void sntx_err(int error), putback(void);
extern void assign_var(char *var_name, int value);
extern int isdelim(char c), look_up(char *s), iswhite(char c);
extern int find_var(char *s), get_token(void);
extern int internal_func(char *s);
extern int is_var(char *s);
extern char *find_func(char *name);
extern void call(void);

void prescan(void);
void decl_global(void), call(void), putback(void);
void decl_local(void), local_push(struct var_type i);
void eval_exp(int *value), sntx_err(int error);
void exec_if(void), find_eob(void), exec_for(void);
void get_params(void), get_args(void);
void exec_while(void), func_push(int i), exec_do(void);
void assign_var(char *var_name, int value);
int load_program(char *p, char *fname), find_var(char *s);
void interp_block(void), func_ret(void);
int func_pop(void), is_var(char *s), get_token(void);
char *find_func(char *name);

#endif