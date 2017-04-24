#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

void mysys(char *argv);
void mysysExec(char *argv);

int main(){
	printf("-----------------------------\n");
	mysys("echo HELLO WORLD");
	printf("-----------------------------\n");
	mysys("ls /");
	mysys("echo -----------------------------\n");
	mysys("pipe");
	mysys(" pwd");
	return 0;
}

char BIN_DIR[] = "/bin/";
// can only reveive 100 argument

char *args[100];

void mysys(char *argv){
	
	pid_t pid = 0;
	pid = fork();
	if(pid == 0){
		mysysExec(argv);
		exit(0);
	}
	else{
		wait();
		// printf("father return ");
		// fflush(0);
	}
	return;
}

void mysysExec(char *argv){
	char* temp_argv = (char*)malloc(sizeof(char)*strlen(argv) + 1);
	strcpy(temp_argv, argv);
	int index = 1;
	int error = 0;
	char *cmd = (char*)strtok(temp_argv," ");
	if(cmd == NULL)
		return;
	char *arg =(char*)strtok(NULL, " ");
	while(arg!=NULL){
		args[index] = (char*)malloc(sizeof(char)*strlen(arg) + 1);
		strcpy(args[index++], arg);
		arg = (char*)strtok(NULL, " ");
	}
	args[0] = cmd;
	args[index++] = NULL;
	// printf("%s",strcat(BIN_DIR, cmd));
	// fflush(0);
	error = execv(strcat(BIN_DIR, cmd), args);
	if (error<0)
		perror("excel");
	return ;
}
