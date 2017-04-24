#include <stdio.h>
#include <unistd.h>

int main(){
	int error;

    puts("before exec");
	char* argv[] = {"echo","test", NULL};
    // error = execl("/bin/echo","echo","test",NULL);
	error = execv("/bin/echo", argv);
    if(error<0){
	        puts("error");
    }
    puts("success");
    return 0;
}

