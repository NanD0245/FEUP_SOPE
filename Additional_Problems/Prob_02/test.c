#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
	char a = getchar();
	char b = getchar();
	printf("%c\n", a);
	printf("%c\n", b);
	return 0;	
}