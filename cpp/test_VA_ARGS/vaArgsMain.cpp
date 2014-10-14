#include <cstdio>

using namespace std;


#ifdef _DEBUG
#define DEBUG_PRINT(format, ...) printf(format, __VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)  
#endif

int main(){

	DEBUG_PRINT("hello, %s", "world");

	getchar();
	return 0;
}