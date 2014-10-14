#include "typedef.h"
#include <stdio.h>

int print(void){
	int i;

	get_token();
	if (*token != '(')  sntx_err(PAREN_EXPECTED);

	get_token();
	if (token_type == STRING){
		printf("%s ", token);
	}else{
		putback();
		eval_exp(&i);
		printf("%d ", i);
	}

	get_token();

	if (*token != ')')  sntx_err(PAREN_EXPECTED);

	get_token();
	if (*token != ';')  sntx_err(SEMI_EXPECTED);
	putback();
	return 0;
}