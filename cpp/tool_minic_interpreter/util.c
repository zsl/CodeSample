#include "typedef.h"
#include <string.h>


int is_var(char *s){
	register int i;
	// 首先看他是否是一个局部变量
	for (i=lvartos-1; i>call_stack[functos-1]; i--){
		if (!strcmp(local_var_stack[i].var_name, token))
			return 1;
	}

	// 否则看它是否是全局变量
	for (i=0; i<NUM_GLOBAL_VARS; i++){
		if (!strcmp(global_vars[i].var_name, s))
			return 1;
	}

	return 0;
}

int internal_func(char *s){
	int i;

	for (i=0; intern_func[i].f_name[0]; i++){
		if (0==strcmp(intern_func[i].f_name, s))
			return i;
	}
	return -1;
}


int isdelim(char c){
	if (strchr(" !;,+-<>'/*%^=()", c) || c==9 ||
		c=='\r' || c==0)
		return 1;
	return 0;
}

int iswhite(char c){
	if (c==' ' || c=='\t')
		return 1;
	return 0;
}