#include "typedef.h"
#include <string.h>

void decl_global(void){
	int vartype;

	get_token();

	vartype = tok;

	do {
		global_vars[gvar_index].v_type = vartype;
		global_vars[gvar_index].value = 0;
		get_token();
		strcpy(global_vars[gvar_index].var_name, token);
		get_token();
		gvar_index++;
	} while(*token == ',');
}

void decl_local(void){
	struct var_type i;

	get_token();

	i.v_type = tok;
	i.value = 0;

	do {
		get_token();
		strcpy(i.var_name, token);
		local_push(i);
		get_token();
	} while(*token == ',');

	if (*token != ';')
		sntx_err(SEMI_EXPECTED);
}