#include"typedef.h"
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>

void putback(void){
	char *t;
	 t = token;
	 for (; *t; t++) prog--;
}

int getnum(void){
	char s[80];

	fgets(s, 80, stdin);
	while (*prog != ')')  prog++;
	prog++;
	return atoi(s);
}

int get_token(void){
	register char *temp;

	token_type = 0;
	tok = 0;
	
	temp = token;

	*temp = '\0';

	// skip over white space
	while (iswhite(*prog) && *prog)  ++prog;

	if (*prog == '\r'){
		++prog;
		++prog;
		// skip over white space
		while (iswhite(*prog) && *prog)  ++prog;
	}

	if (*prog == '\0'){// end of file
		*token = '\0';
		tok = FINISHED;

		return (token_type = DELIMITER);
	}

	// block delimiters
	if (strchr("{}", *prog)){
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;

		return (token_type = BLOCK);
	}

	// look for comments
	if (*prog == '/'){
		if (*(prog+1) == '*'){// is a comment
			prog += 2;
			do{// find end of comment
				while (*prog != '*') prog++;
				prog++;
			}while(*prog!='/');
			prog++;
		}
	}

	// is or might be a relational operator
	if (strchr("!<>=", *prog)){
		switch (*prog){
		case '=':
			if (*(prog+1)=='='){
				prog++;
				prog++;
				*temp = EQ;
				temp++;
				*temp = EQ;
				temp++;
				*temp = '\0';
			}
			break;
		case '!':
			if (*(prog+1)=='='){
				prog++;
				prog++;
				*temp = NE;
				temp++;
				*temp = NE;
				temp++;
				*temp = '\0';
			}
			break;
		case '<':
			if (*(prog+1)=='='){
				prog++;
				prog++;
				*temp = LE;
				temp++;
				*temp = LE;
			}else{
				prog++;
				*temp = LT;
			}
			temp++;
			*temp = '\0';
			break;
		case '>':
			if (*(prog+1)=='='){
				prog+=2;
				*temp = GE;
				*++temp = GE;
			}else{
				prog++;
				*temp = GT;
			}
			* ++temp = '\0';
			break;
		}
		if (*token) return (token_type = DELIMITER);
	}

	if (strchr("+-*^/%=;(),'", *prog)){
		*temp = *prog;
		prog++;
		temp++;
		*temp='\0';
		return (token_type = DELIMITER);
	}

	if (*prog == '"'){
		prog++;
		while (*prog !='"' && *prog!='\r') *temp++ = *prog++;
		if (*prog == '\r') sntx_err(SYNTAX);
		prog++;
		*temp = '\0';
		return (token_type=STRING);
	}

	if (isdigit(*prog)){
		while (!isdelim(*prog)) *temp++ = *prog++;
		*temp = '\0';
		return (token_type = NUMBER);
	}

	if (isalpha(*prog)){
		while (!isdelim(*prog)) *temp++ = *prog++;
		token_type = TEMP;
	}

	*temp = '\0';

	if (token_type == TEMP){
		tok = look_up(token);

		if (tok) token_type = KEYWORD;
		else token_type = IDENTIFIER;
	}
	return token_type;
}