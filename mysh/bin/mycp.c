#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	char* dest_file = argv[2];
	char* src_file = argv[1];
	
	int src_file_handle = open(src_file, O_RDONLY);
	int dest_file_handle = open(dest_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	
	char temp = 0;
	while(read(src_file_handle, &temp, 1)){
		// printf("%c", temp);
		write(dest_file_handle, &temp, 1);
		// fflush(dest_file_handle);
	}
	close(src_file_handle);
	close(dest_file_handle);

}
