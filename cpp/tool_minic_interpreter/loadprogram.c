#include "typedef.h"
#include <stdio.h>

int load_program(char *p, char *fname){
	FILE *fp;
	int i = 0;

	if ((fp=fopen(fname, "rb"))==NULL) return 0;

	i = 0;
	do {
		*p = getc(fp);
		p++; i++;
	} while(!feof(fp) && i<PROG_SIZE);

	if (*(p-1) == EOF) 
		*(p-1) = '\0';
	fclose(fp);

	return 1;
}