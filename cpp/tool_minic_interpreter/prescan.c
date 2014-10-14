#include "typedef.h"
#include <string.h>

// 找到main函数并标记位置
// 找到所有全局变量并分配空间
void prescan(void){
	char *p, *tp;
	char temp[32];
	int datatype;
	int brace = 0;

	p = prog;
	func_index = 0;
	do{
		while (brace){
			get_token();
			if (*token == '{') brace++;
			if (*token == '}') brace--;
		}

		tp = prog; // 保存当前位置
		get_token();

		if (tok==CHAR || tok==INT){
			datatype = tok;
			get_token();
			if (token_type == IDENTIFIER){
				strcpy(temp, token);
				get_token();
				if (*token != '('){// 若是全局数据
					prog =  tp;
					decl_global();
				}
				else if (*token == '('){
					func_table[func_index].loc = prog;
					func_table[func_index].ret_type = datatype;
					strcpy(func_table[func_index].func_name, temp);
					func_index++;
					while (*prog != ')') prog++;
					prog++;
				}
				else
					putback();
			}
		}
		else if (*token == '{')
			brace++;

	}while(tok!=FINISHED);
	prog = p;
}