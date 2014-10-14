#include "typedef.h"
#include <string.h>
#include <stdlib.h>

void find_eob(void){
	int brace;

	get_token();
	brace = 1;
	do {
		get_token();
		if (*token == '{')
			brace++;
		else if (*token == '}')
			brace--;
	} while(brace);
}

void interp_block(void){
	int value;
	char block = 0;

	do {
		token_type == get_token();

		if (token_type == IDENTIFIER){
			putback();
			eval_exp(&value);
			if (*token != ';')  sntx_err(SEMI_EXPECTED);
		}
		else if (token_type == BLOCK){
			if (*token == '{') block = 1;
			else return;
		}
		else{ // is keyword
			switch (tok){
			case CHAR:
			case INT:
				putback();
				decl_local();
				break;
			case RETURN:
				func_ret();
				return;
			case IF:
				exec_if();
				break;
			case ELSE:
				find_eob();
				break;
			case WHILE:
				exec_while();
				break;
			case DO:
				exec_do();
				break;
			case FOR:
				exec_for();
				break;
			case END:
				exit(0);
			}
		}
	} while(tok!=FINISHED && block);
}

void exec_if(void){
	int cond;

	// get if expression
	eval_exp(&cond); 

	if (cond){
		interp_block();
	}else{
		// find start of next line
		find_eob();
		get_token();

		if (tok != ELSE){
			// restore token if no ELSE is present
			putback();
			return;
		}
		interp_block();
	}
}

void exec_while(void){
	int cond;
	char *temp;

	putback();
	// save location of top of while loop
	temp = prog;
	get_token();
	eval_exp(&cond);

	if (cond){
		interp_block();
	}else{
		find_eob();
		return;
	}
	prog = temp;
}

void exec_do(void){
	int cond;
	char *temp;

	putback();
	temp = prog;

	get_token();
	interp_block();
	get_token();
	if (tok != WHILE)
		sntx_err(WHILE_EXPECTED);
	eval_exp(&cond);
	if (cond) prog = temp;
}

void exec_for(void){
	int cond;
	char *temp, *temp2;
	int brace;

	get_token();
	// initialization expression
	eval_exp(&cond);
	
	if (*token != ';')
		sntx_err(SEMI_EXPECTED);
	// get past the ;
	prog++;
	temp = prog;

	// find the start of the for block
	brace = 1;
	while (1){
		eval_exp(&cond);
		if (*token != ';')
			sntx_err(SEMI_EXPECTED);
		// get past the ;
		prog++; 
		temp2 = prog;

		// find the start of the for block
		brace = 1;
		while (brace){
			get_token();
			if (*token == '(')
				brace++;
			if (*token == ')')
				brace--;
		}

		if (cond) interp_block();
		else{
			find_eob();
			return;
		}
		prog = temp2;
		eval_exp(&cond);
		prog = temp;
	}
}