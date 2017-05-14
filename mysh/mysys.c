#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void mysys(char *argv);
void mysysExec(char *argv);
/*
int main(){
	printf("-----------------------------\n");
	mysys("echo HELLO WORLD");
	printf("-----------------------------\n");
	mysys("ls /");
	mysys("echo -----------------------------\n");
	return 0;
}
*/

char BIN_DIR[] = "/bin/";
char MY_DIR[] = "./bin/";
# define DEBUG 1

// can only reveive 100 argument

char *args[100];

void mysys(char *argv){
	
	pid_t pid = 0;
	pid = fork();
	// printf("mysys the readline is %s", argv);
	// fflush(0);
	if(pid == 0){
		mysysExec(argv);
		exit(0);
	}
	else{
		wait(NULL);
		// printf("father return ");
		// fflush(0);
	}
	return;
}

void mysysExec(char *argv){

	// printf("myExec the readline is %s\n", argv);
	fflush(0);
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
#if DEBUG == 0
	args[0] = (char*)malloc(sizeof(char)*(strlen(cmd) + strlen(MY_DIR)+2));
	strcpy(args[0], "my");
	strcat(args[0], cmd);
#else
	args[0] = cmd;
#endif
	args[index++] = NULL;
	// printf("cmd %s, args is %s",strcat(BIN_DIR, cmd),args[0]);
	// fflush(0);
	// error = execv(strcat(BIN_DIR, cmd), args);
# if DEBUG == 1
	error = execv(strcat(BIN_DIR, cmd), args);
# elif DEBUG == 0
	// printf("now path is %s\n", strcat(MY_DIR, args[0]));
	// int i = 0;
	args[0] = strcat(MY_DIR, args[0]);
	/*
	for(i = 0;i<index;i++){
		printf("argv[%d] %s\n", i, args[i]);
	}
	*/
	// error = execv(strcat(MY_DIR, args[0]), args);
	// error = execv(args[0], args);
# endif
	error = execvp(cmd, args);
	fflush(0);
	if (error<0)
		perror("excel");
	return ;
}
