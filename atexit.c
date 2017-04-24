#include <stdlib.h>

void fun1(){
	puts("f1");
}

void fun2(){
	puts("f2");
}

void fun3(){
	puts("f3");
}
int main(){
	
	atexit(fun1);
	atexit(fun2);
	atexit(fun3);

	return 0;
}
