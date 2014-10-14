#include <stdio.h>
#include <setjmp.h>

jmp_buf jmpbuffer;

int main()
{
    int c;
    int ret;
    char buf[256];

    ret = setjmp(jmpbuffer);
    // setjmp ±æ…Ì∑µªÿ0
    if ( ret != 0){
        if (1 == ret){
            printf("game over ;)\n");
            exit(0);
        }
    }
    printf("please input:");
    if ( gets(buf) ){
        printf("%s\n", buf);
        longjmp(jmpbuffer, 2);
    }
    else
        longjmp(jmpbuffer, 1);

    return 0;
}
