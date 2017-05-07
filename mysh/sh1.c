#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
// #include <readline/readline.h>
// #include <readline/history.h>

extern void mysys(char *argv);

char* readline(char* s){
	printf("%s", s);
	fflush(0);
	// char t;
	char *line = (char*)malloc(sizeof(char)*80);
	// int index = 0;
	read(0, line, 80);
	int t = 0;
	for(t = 0; line[t]!= '\n'; t++);
	line[t] = '\0';
	/*
	while(read(0, &t, 1)&&index<79){
		line[index++] = t;
	}
	*/
	// printf("%s", line);
	// fflush(0);
	// line[index] = '\0';
	return line;
}

void add_history(char* line){
	int fd = open("mylog.txt", O_RDWR| O_APPEND|O_CREAT, 0666);
	char* end = "\n";
	if(fd <0){
		perror("Can't not open mylog.txt");
	}
	if(write(fd, line, 80)<0){
		perror("Write to log.txt error");
	}
	write(fd, end, 1);
	return ;
}
char* myreadline(){
	static char* line_read = NULL;
	if(line_read){
		free(line_read);
		line_read = NULL;
	}
	line_read = readline("$ ");
	int length = strlen(line_read);
	line_read[length] = '\0';
	/*
	if (line_read && *line_read){
		add_history(line_read);
	}
	*/
	return line_read;
}

/* *************************
 * BuildIn
 * to realize cmd with build program
 * @char* str:input str
 * if exitst, will return 1,else return 0
 *
 * *************************
 */
int BuildIn(char* str){
	char *argv;
	int exist = 0;
	// printf("result %s.", str);
	// int i = strcpy(str, "exit");
	// printf("i is %d",i);
	if(str[0] == 'c' && str[1] == 'd'){
		argv = &str[3];
		int fd;
		if((fd = open(argv, O_RDONLY)) <0){
			perror("oops... can not open");
			return 0;
		}
		fchdir(fd);
		exist = 1;
	}
	else if (strcmp(str, "exit")==0){
		// printf("exit cmd\n");
		exit(0);
	}
	return exist;
}
int main(int argc , char* argv[]){
	int i = 0;
	int result = 0;
	while(1){
		char *str = myreadline();
		i = 0;
		// first ,check whether is build_program
		result = BuildIn(str);
		if(result ==0)
		// check if is shell program or system program
			mysys(&str[i]);
	}
	return 0;
}
