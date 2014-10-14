#include "typedef.h"
#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 解释器的入口点
void eval_exp(int *value){
	get_token();
	if (! *token){
		sntx_err(NO_EXP);
		return;
	}

	if (*token == ';'){
		*value = 0;  // empty expression
		return;
	}

	eval_exp0(value);
	putback();  // return last token read to input stream
}

// 处理赋值表达式
void eval_exp0(int *value){
	char temp[ID_LEN];  // 

	register int temp_tok;

	if (token_type == IDENTIFIER){
		if (is_var(token)){
			strcpy(temp, token);
			temp_tok = token_type;
			get_token();
			if (*token == '='){//如果是一个赋值表达式
				get_token();
				eval_exp0(value);
				assign_var(temp, *value); // 赋值
				return;
			}else{ //不是赋值表达式
				putback();
				strcpy(token, temp);
				token_type = temp_tok;
			}
		}
	}
	eval_exp1(value);
}


// 执行关系表达式
void eval_exp1(int *value){
	int partial_value;
	register char op;
	char relops[7] = {LT, LE, GT, GE, EQ, NE, 0};

	eval_exp2(value);
	op = *token;
	if (strchr(relops, op)){
		get_token();
		eval_exp2(&partial_value);
		switch (op){
		case LT:
			*value = *value < partial_value;
			break;
		case LE:
			*value = *value <= partial_value;
			break;
		case GT:
			*value = *value > partial_value;
			break;
		case GE:
			*value = *value >= partial_value;
			break;
		case EQ:
			*value = *value == partial_value;
			break;
		case NE:
			*value = *value != partial_value;
			break;
		}
	}
}

void eval_exp2(int *value){
	register char op;
	int partial_value;

	eval_exp3(value);
	while ((op = *token)== '+' || op=='-'){
		get_token();
		eval_exp3(&partial_value);
		switch (op){
		case '-':
			*value = *value - partial_value;
			break;
		case '+':
			*value = *value + partial_value;
			break;
		}
	}
}

void eval_exp3(int *value){
	register char op;
	int partial_value, t;

	eval_exp4(value);
	while ((op = *token)=='*' || op=='/' || op=='%'){
		get_token();
		eval_exp4(&partial_value);
		switch (op){
		case '*':
			*value = *value *partial_value;
		case '/':
			if (partial_value == 0)
				sntx_err(DIV_BY_ZERO);
			*value = *value / partial_value;
			break;
		case '%':
			t = *value / partial_value;
			*value = *value - t * partial_value;
			break;
		}
	}
}

void eval_exp4(int *value){
	register char op;

	op = '\0';
	if (*token == '+' || *token == '-'){
		op = *token;
		get_token();
	}
	eval_exp5(value);
	if (op){
		if (op == '-')  *value = -(*value);
	}
}

/* Process parenthesized expression. */
void eval_exp5(int *value){
	if (*token == '('){
		get_token();
		eval_exp0(value);
		if (*token != ')') 
			sntx_err(PAREN_EXPECTED);
		get_token();
	}else{
		atom(value);
	}
}

void atom(int *value){
	int i;

	switch(token_type) {
	case IDENTIFIER:
		i = internal_func(token);
		if (i != -1){ // 调用标准程序库函数
			*value = (*intern_func[i].p)();
		}else if (find_func(token)){
			call();
			*value = ret_value;
		}else{
			*value = find_var(token);
		}
		get_token();
		return;
	case NUMBER:
		*value = atoi(token);
		get_token();
		break;
	case DELIMITER:
		if (*token == '\''){
			*value = *prog;
			prog++;
			if (*prog != '\'')
				sntx_err(QUOTE_EXPECTED);
			prog++;
			get_token();
			return;
		}
		if (*token == ')') return;
		else sntx_err(SYNTAX);
	default:
		sntx_err(SYNTAX);
	}
}

void sntx_err(int error){
	char *p, *temp;
	int linecount = 0;
	register int i;

	static char *e[] = {
		"syntax error",
		"unbalanced parentheses",
		"no expression present",
		"equal sign expected",
		"not a variable",
		"parmeter error",
		"semicolon expected",
		"unbalanced braces",
		"function undefined",
		"type specifier expected",
		"too many nested function calls",
		"return without call",
		"parentheses expected",
		"while expected",
		"closing quote expected",
		"not a string",
		"too many local variables",
		"division by zero"
	};

	printf("\n%s", e[error]);
	p = p_buf;
	while (p != prog){
		p++;
		if (*p == '\r'){
			linecount++;
		}
	}
	printf(" in line %d\n", linecount);

	temp = p;
	for (i=0; i<20 && p>p_buf && *p != '\n'; i++, p--);
	for (i=0; i<30 && p<=temp; i++, p++)
		printf("%c", *p);
	longjmp(e_buf, 1);

}