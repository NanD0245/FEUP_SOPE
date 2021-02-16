#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
	int	f1 = open("example.txt",O_WRONLY);
	int a = dup(f1);
	write(a,"Ola example.txt\n",16);
	return 0;
}