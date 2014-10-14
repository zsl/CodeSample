#include "typedef.h"
#include <ctype.h>
#include <string.h>

int look_up(char *s){
	register int i;
	char *p;

	p = s;
	while (*p){
		*p = tolower(*p);
		p++;
	}

	for (i=0; *table[i].command; i++){
		if (0 == strcmp(table[i].command, s))
			return table[i].tok;
	}
	return 0;
}