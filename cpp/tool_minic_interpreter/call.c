#include "typedef.h"
#include <string.h>
#include <stdio.h>

void call(void){
	char *loc, *temp;
	int lvartemp;

	loc = find_func(token);
	if (loc == NULL)
		sntx_err(FUNC_UNDEF);
	else{
		lvartemp = lvartos;
		get_args();
		// save return function 
		temp = prog;    
		// save local var stack index
		func_push(lvartemp);  
		// reset prog to start
		prog = loc;  
		// load the function's parameter with the values of the arguments
		get_params();
		// interpret the function
		interp_block();
		// reset the program pointer
		prog = temp;
		// reset the local var stack
		lvartos = func_pop();
	}
}

void func_ret(void){
	int value;

	value = 0;
	// get return value, if any
	eval_exp(&value);
	ret_value = value;
}

int func_pop(void){
	functos--;
	if (functos < 0)  sntx_err(RET_NOCALL);

	return call_stack[functos];
}

void func_push(int i){
	if (functos > NUM_FUNC)
		sntx_err(NEST_FUNC);
	call_stack[functos] = i;
	functos++;
}

void get_args(void){
	int value, count, temp[NUM_PARAMS];
	struct var_type i;

	count = 0;
	get_token();
	if (*token != '(')
		sntx_err(PAREN_EXPECTED);

	// process a comma-separated list of values
	do {
		eval_exp(&value);
		temp[count] = value;
		get_token();
		count++;
	}while(*token == ',');
	count--;

	// push on local_var_stack in reverse order
	for (; count>=0; count--){
		i.value = temp[count];
		i.v_type = ARG;
		local_push(i);
	}
}

void get_params(void){
	struct var_type *p;
	int i;

	i = lvartos-1;
	do {
		get_token();
		p = &local_var_stack[i];
		if (*token != ')'){
			if (tok!=INT && tok!=CHAR)
				sntx_err(TYPE_EXPECTED);

			p->v_type = token_type;
			get_token();

			// link parameter name with argument already
			strcpy(p->var_name, token);
			get_token();
			i--;
		}
		else break;
	} while(*token == ',');
}

int call_getche(){
	char ch;
	ch = getchar();
	while (*prog != ')') prog++;
	prog++;
	return ch;
}

int call_putch(){
	int value;

	eval_exp(&value);
	printf("%c", value);
	return value;
}

int call_puts(){
	get_token();
	if (*token!='(') sntx_err(PAREN_EXPECTED);
	get_token();
	if (token_type!=STRING) sntx_err(QUOTE_EXPECTED);
	puts(token);
	get_token();
	if (*token!=')') sntx_err(PAREN_EXPECTED);

	get_token();
	if (*token != ';') sntx_err(SEMI_EXPECPED);

	putback();
	return 0;
}