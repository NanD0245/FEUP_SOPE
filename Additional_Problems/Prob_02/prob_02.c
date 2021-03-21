#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>


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



//4
int f4a() {
    int f1 = open("students.txt",O_WRONLY | O_TRUNC);
    char name[51],grade[4],c; //"xx\n\0"
    while (1) {
        memset(name,0,sizeof(name));
        memset(name,0,sizeof(grade));

        printf("Student name (max 50 chars): ");
        for (int i = 0; ((c=getchar()) != '\n' && c != EOF) && i < 50 ; i++) {
            name[i] = c;
        }
        if (c != '\n') 
            while ((c=getchar()) != '\n');
        name[strlen(name)]=' ';
        name[strlen(name)]='\0';

        printf("Grade (0-20): ");
        for (int i = 0; (c=getchar()) != '\n' && c != EOF && i < 2 ; i++) {
            grade[i] = c;
        }
        if (c != '\n') 
            while ((c=getchar()) != '\n');
        printf("%ld\n", strlen(grade));
        grade[2]='\n';
        grade[3]='\0';
        write(f1,name,strlen(name));
        write(f1,grade,strlen(grade));
    }
    return 0;
}

struct student {
    char* name;
    int grade;    
};


int f4b() {
    int f1 = open("students.txt",O_WRONLY | O_TRUNC);
    char name[51],grade[4],c; //"xx\n\0"
    struct student s;
    while (1) {
        memset(name,0,sizeof(name));
        memset(name,0,sizeof(grade));

        printf("Student name (max 50 chars): ");
        for (int i = 0; ((c=getchar()) != '\n' && c != EOF) && i < 50 ; i++) {
            name[i] = c;
        }
        if (c != '\n') 
            while ((c=getchar()) != '\n');
        name[strlen(name)]=' ';
        name[strlen(name)]='\0';

        printf("Grade (0-20): ");
        for (int i = 0; (c=getchar()) != '\n' && c != EOF && i < 2 ; i++) {
            grade[i] = c;
        }
        if (c != '\n') 
            while ((c=getchar()) != '\n');
        printf("%ld\n", strlen(grade));
        grade[2]='\n';
        grade[3]='\0';
        s.name = name;
        s.grade = atoi(grade);
        write(f1,&s,sizeof(struct student));
    }
}
//não é possivel usar o cat pois o ficheiro é binário e não de texto



//5
int p5a(void)
{
    int fd;
    char *text1="AAAAA";
    char *text2="BBBBB";
    fd = open("f1.txt",O_CREAT|O_EXCL|O_TRUNC|O_WRONLY|O_SYNC,0600); //Só escreve no ficheiro se o ficheiro não estiver criado
    write(fd,text1,5);
    write(fd,text2,5);
    close(fd);
    return 0;
}

int p5b(void)
{
    int fd;
    char *text1="CCCCC";
    char *text2="DDDDD";
    fd = open("f1.txt", O_WRONLY|O_SYNC); //escreve sempre no ficheiro (sobrepõe ao que está lá)
    write(fd,text1,5);
    write(fd,text2,5);
    close(fd);
    return 0;
}



//6
int f6(int argc, char *argv[])
{
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    char *str;
    if (argc != 2) { 
        fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }
    if ((dirp = opendir(argv[1])) == NULL) {
        perror(argv[1]);
        exit(2);
    }
    while ((direntp = readdir(dirp)) != NULL) {
        //direntp = readdir(dirp);
        if (lstat(direntp->d_name, &stat_buf) != 0) {
            printf("%s\n", direntp->d_name);
            perror("Error:"); //a)
        }
        if (S_ISREG(stat_buf.st_mode)) str = "regular";
        else if (S_ISDIR(stat_buf.st_mode)) str = "directory";
        else if (S_ISLNK(stat_buf.st_mode)) str = "link";
        else str = "other";
        printf("%-25s - %s - %ld - st_mode = %d\n", direntp->d_name, str, direntp->d_ino, stat_buf.st_mode); //b)
    }
    closedir(dirp);
    exit(0);
}
//c)
//hardlink -> same type of source
//symbolic link -> type: symbolic link

//d)
//lstat -> if is a symbolic links return the info about the symbolic link
//stat -> if is a symbolic links return the info about the link itself

//e)
//ao eliminar o temp.txt o temp2.txt é como se não existi-se mas o temp1.txt continua


int main(int argc, char* argv[], char* envp[]) {
	return f6(argc, argv);
}