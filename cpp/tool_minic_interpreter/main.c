#include "typedef.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	/*
	if (argc != 2){
		printf("Usage: littlec <filename>\n");
		exit(1);
	}
	*/
	if (NULL == (p_buf=(char*)malloc(PROG_SIZE))){
		printf("Allocation Failure");
		exit(1);
	}

	// load the program to execute
	//if (!load_program(p_buf, argv[1]))  exit(1);
	if (!load_program(p_buf, "d:/test0.lc"))  exit(1);
	if (setjmp(e_buf))  exit(1);

	gvar_index = 0;

	prog = p_buf;
	prescan();

	lvartos = 0;
	functos = 0;

	prog = find_func("main");

	if (!prog){
		printf("main() not found");
		exit(1);
	}

	prog--;
	strcpy(token, "main");
	call();

	printf("\n程序执行结束，请按回车退出控制台：");
	getchar();
	return;
}