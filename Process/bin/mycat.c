#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

	char *filename = argv[1];
	int file = open(filename, O_RDONLY);
	char temp = 0;
	while(read(file, &temp, 1)){
		write(0, &temp, 1);
	}
	return 0;
}
