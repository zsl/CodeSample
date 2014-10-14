#include "typedef.h"
#include <string.h>

void local_push(struct var_type i){
	if (lvartos > NUM_LOCAL_VARS)
		sntx_err(TOO_MANY_LVARS);

	local_var_stack[lvartos] = i;
	lvartos++;
}

void assign_var(char *var_name, int value){
	register int i;

	for (i=lvartos-1; i>=call_stack[functos-1]; i--){
		if (0 == strcmp(local_var_stack[i].var_name, var_name)){
			local_var_stack[i].value = value;
			return;
		}
	}
	if (i < call_stack[functos-1]){
		for (i=0; i<NUM_GLOBAL_VARS; i++)
			if (0==strcmp(global_vars[i].var_name, var_name)){
				global_vars[i].value = value;
				return;
			}
	}
	sntx_err(NOT_VAR);
}

int find_var(char *s){
	register int i;

	// local variable
	for (i=lvartos-1; i >= call_stack[functos-1]; i--){
		if (!strcmp(local_var_stack[i].var_name, token))
			return local_var_stack[i].value;
	}

	// global variable
	for (i=0; i < NUM_GLOBAL_VARS; i++){
		if (!strcmp(global_vars[i].var_name, s))
			return global_vars[i].value;
	}

	sntx_err(NOT_VAR);
	return -1;
}