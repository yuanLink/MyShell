#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
// #include <readline/readline.h>
// #include <readline/history.h>

extern void mysys(char *argv);
char build_in_char[] = "|<>";

char* readline(char* s){
	printf("%s", s);
	fflush(0);
	// char t;
	char *line = (char*)malloc(sizeof(char)*80);
	memset(line, '\0', 80);
	// int index = 0;
	read(0, line, 80);
	int t = 0;
	for(t = 0; line[t]!= '\n'&&t!=79; t++);
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

/********************************
 *			flowCheck
 * check each flow and change it .
 * In this function ,it will create a pipe, use p[0] to save stdin and 
 * p[1] to save stdout.
 * 
 * param str: we will check whether it is has flow change float
 * 
 * return a pipe call fd, fd[0] default is stdin, fd[1] is stdout
 ********************************/
int* flowCheck(char *str){
	static int fd[2];
	int temp_fd;
	char* filename = malloc(sizeof(char)*40);
	// pipe(fd);
	fd[0] = dup(0);fd[1] = dup(1);
	int length = strlen(str);
	int i = 0, j = 0;
	for(i = 0; i < length; i++){
		// if this is a stdin change 
		if(str[i] == '<'){
			i++;
			// find the begin of this str
			while(str[i] == ' ') i++;
			// find the end of this str
			j = i;
			while(str[j]!=' '&&(str[j]!='<'||str[j]!='>')&&str[j]!='\0')j++;
			memcpy(filename, &str[i], j - i);
			temp_fd = open(filename, O_CREAT|O_RDONLY, 0666);
			fd[0] = temp_fd;
		}
		// stdout change
		else if(str[i] == '>'){
			i++;
			// find the begin of this str
			while(str[i] == ' ') i++;
			// find the end of this str
			j = i;
			while(str[j]!=' '&&(str[j]!='<'||str[j]!='>')&&str[j]!='\0')j++;
			memcpy(filename, &str[i], j - i);
			temp_fd = open(filename, O_CREAT|O_WRONLY, 0666);
			fd[1] = temp_fd;
		}
	}
	free(filename);
	return fd;
}
/********************************
*			getCmd
* get first words as cmd
* 
* param str: the input line
* 
* return a str that we check it as cmd
* CAUTION! return str should be free
**********************************/
char* getCmd(char* str){
	char* cmd = (char*)malloc(sizeof(char)*30);
	memset(cmd, '\0', 30);
	int i = 0;
	int length = strlen(str);
	// int j = 0;
	for(i = 0; i < length; i++){
		if(str[i]=='|'||str[i]=='<'||str[i]=='>')
			break;
	}
	memcpy(cmd, str, i);
	return cmd;	
}

/********************************
 *			getChar
 * get char index if exist ,else return -1
 *
 * char* str: the str that we need to find 
 * char* chr: chr that we need to find
 *
 * return: if find , it will return the first time it appear,
 * else it will return -1
 *
 *******************************/
int getChar(char* str, char chr){
	int index = 0, length = strlen(str);
	for(index = 0; index<length; index++){
		if(str[index] == chr)
			return index;
	}
	return -1;
}
int main(int argc , char* argv[]){
	// int i = 0;
	int result = 0;
	int outTemp, inTemp;
	int* fds;
	int p[2], *p_;
	p[0] = 0;p[1] = 1;
	p_ = NULL;
	int pos = 0, index = 0;
	char temp[1024];
	// char* path;
	while(1){
		char* str = myreadline();
		char* cmd = getCmd(str);
		// first ,check whether is build_program
		result = BuildIn(str);
		fds = flowCheck(str);
		index = 0;
		// printf("result is %d\n",result);
		if(result ==0){
			// redirect pipe
			inTemp = dup(0);
			dup2(fds[0], 0);
			close(fds[0]);

			outTemp = dup(1);
			// dup2(fds[1], 1);
			// we should check whether there are pipe
			pos = getChar(&str[index], '|');
			// printf("pos is %d\n", pos);
			while(pos!=-1){
				// give a pipe to it 
				pipe(p);
				//write(outTemp, "create pipe\n", 12);
				// change stdout to this pipe output
				dup2(p[1], 1);
				close(p[1]);
				// get cmd and try to execute
				cmd = getCmd(&str[index]);
				// write(outTemp, cmd, sizeof(cmd)+ 6);
				// write(outTemp, "\n", 1);
				index = pos + 1;
				// write to pipe
				mysys(cmd);
				// read(1, temp, 10);
				// write(outTemp, temp ,10);
				// fflush(0);
				// if p_ is not empty , remember to close last write pipe
				if(p_ != NULL){
					close(p_[1]);
					close(p_[0]);
				}
				// this pipe has write a new infomation ,then we need to lets next cmd to read it 
				dup2(p[0], 0);
				// use p_ to record last pipe
				p_ = p;
				// close read pipe
				close(p[0]);
				pos = getChar(&str[index],'|');
				// write(outTemp, "\ncontinue", 9);
			}
			// close(fds[0]);
			dup2(fds[1], 1);
			close(fds[1]);
			cmd = getCmd(&str[index]);
			// write(fds[1], "\nout...", 7);
			// write(outTemp, cmd, 7);
			// read(0, temp ,120);
			// write(outTemp, temp ,120);
			mysys(cmd);

			/*
			if(fds[1]!=1);
				close(fds[1]);
			if(fds[0]!=0)
				close(fds[0]);
			*/
			// finally ,return pipe
			dup2(inTemp, 0);
			close(inTemp);
			dup2(outTemp,1);
			close(outTemp);

			printf("pos is %d, now cmd is %s\n",pos, cmd);
			// and free cmd
			free(cmd);
			// write(fds[1], "\nfinish", 7);
		}
	}
	return 0;
}
