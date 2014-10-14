#include "typedef.h"
#include <string.h>

char *find_func(char *name){
	register int i;

	for (i=0; i < func_index; i++){
		if (!strcmp(name, func_table[i].func_name))
			return func_table[i].loc;
	}

	return NULL;
}