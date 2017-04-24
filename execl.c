#include <stdio.h>
#include <unistd.h>

int main(){
	int error;

	puts("before exec");
	error = execl("/bin/echo","echo","test",NULL);
	if(error<0){
		puts("error");
	}
	puts("success");
	return 0;
}
