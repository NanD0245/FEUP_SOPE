#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int f1() {
	struct termios old,new;
    if (tcgetattr(STDIN_FILENO, &old) != 0) {
        perror("tcgetattr failed");
        exit(-1);
    }

    new = old;
    new.c_lflag &= ~ICANON; //disables canonical mode (input is made char by char instead of line by line) 
    						//problem: disables backspace

    new.c_lflag &= ~ECHO; //disacbles echo
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new) != 0) {
        perror("tcsetattr failed");
        exit(-1);
    }

	char buf[128], c = '\0';
	int size = 128;
    while (size > 0 && (c = getchar()) != '\n') {
    	if (c == 127 && strlen(buf)>1) {
			printf("\b \b");
			size++;
			buf[-size+128] = '\0';
    	}
    	else {
        	buf[-size+128] = c;
        	size--;
        	putchar('*');
        }
    }
    buf[size] = '\0';
    printf("\nPass: %s\n", buf);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &old) != 0) {
        perror("tcsetattr failed");
        exit(-1);
    }
    return 0;
}



int f2a(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: nome_do_executavel fsource fdestination\n");
        return 0;
    }
    FILE* f1,*f2;
    if ((f1 = fopen(argv[1],"r")) == NULL) {
        printf("Error: file doesn't exist!\n");
    }
    f2 = fopen(argv[2],"w");
    char buf[256];
    if (fgets(buf,256,f1) == NULL)
        perror("ERROR");
    while (fgets(buf,256,f1) != NULL) {
        fputs(buf,f2);
    }
    return 0;
}

int f2b(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: nome_do_executavel fsource fdestination\n");
        return 0;
    }
    int f1 = open(argv[1],O_RDONLY);
    int f2 = open(argv[2],O_WRONLY | O_TRUNC);
    if (f1 == 0) {
        printf("file doesn't exist\n");
        return -1;
    }
    char buf[256];
    while (read(f1,buf,256) > 0) {
        write(f2,buf,strlen(buf));
        memset(buf,0,strlen(buf));
    }
    return 0;
}



//3
int f3a(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: nome_do_executavel fsource\n");
        return 0;
    }
    int f1 = open(argv[1],O_RDONLY);
    if (f1 == 0) {
        printf("file doesn't exist\n");
        return -1;
    }
    char buf[256];
    while (read(f1,buf,256) > 0) {
        printf("%s", buf);
        memset(buf,0,sizeof(buf));
    }
    return 0;
}

int f3b(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: nome_do_executavel fsource fdestination\n");
        return 0;
    }
    int f1 = open(argv[1],O_RDONLY);
    int f2 = open(argv[2],O_WRONLY | O_TRUNC);
    if (f1 == 0) {
        printf("file doesn't exist\n");
        return -1;
    }
    char buf[256];
    dup2(f2,1);
    while (read(f1,buf,256) > 0) {
        printf("%s", buf);
        memset(buf,0,sizeof(buf));
    }
    return 0;
}


int main(int argc, char* argv[], char* envp[]) {
	//return f1();
    //return f2a(argc,argv);
    //return f2b(argc,argv);
    //return f3a(argc, argv);
    //return f3b(argc,argv); 
}