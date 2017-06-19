#include <stdio.h>
#include <unistd.h>

// cat /etc/passwd |wc -l

int main()
{
	int pid;
    int fd[2];
    char buf[32];

    pipe(fd);
	/*
    write(fd[1], "hello", 6);
    read(fd[0], buf, sizeof(buf));
	*/
	pid = fork();

	if(pid == 0){
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);

		execlp("cat", "cat", "/etc/passwd", NULL);
		exit(0);
	}
	dup2(fd[0],0);
	close(fd[0]);
	close(fd[1]);

	execlp("wc", "wc", "-l", NULL);
    printf("Receive:%s\n", buf);
	printf("read pipe:%d, write pipe:%d\n", fd[0], fd[1]);
    return 0;
}
