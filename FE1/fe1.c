#include <stdio.h>
#include <string.h>
#include <stdlib.h> //atexit();
#include <unistd.h> //function calls
#include <sys/times.h> //tms
#include <fcntl.h> //O_RDONLY; O_WRONLY
#include <errno.h> //errno
#include <dirent.h> //dirent
#include <sys/stat.h> //stat


/*1.
a)
False - It's not just for C source code.


b)
A flag -Wall serve para, ao compilar o programa, mostrar warnings do código que pode levar a problemas de execução.


c)
ver makefile.



2.

A dinâmica utiliza menos espaço mas está mais suscetível a falhas, dado que se a biblioteca falhar o executável falha.

No caso da estática, uma modificação da biblioteca obriga a recompilar. No entanto, é mais rápido e tem uma melhor performance por não haver
necessidade de recorrer a ficheiros externos. Ao não recorrer a ficheiros externos a sua mobilidade também é maior



3
a)
O serviço atexit() não é útil quando o programa é terminado de repente pois apenas é chamado quando o programa termina normalmente.


//3 EXAMPLE
#include <stdlib.h>
#include <unistd.h>

void
bye(void)
{
   printf("That was all, folks\n");
}

int
main(void)
{
   long a;
   int i;

   a = sysconf(_SC_ATEXIT_MAX);
   printf("ATEXIT_MAX = %ld\n", a);

   i = atexit(bye);
   if (i != 0) {
       fprintf(stderr, "cannot set exit function\n");
       exit(EXIT_FAILURE);
   }

   exit(EXIT_SUCCESS);
}*/

//3
void f3example() {
	printf("FUNCTIONâsdasd OPENED!\n");
}

int f3() {
	atexit(f3example);
	printf("Waiting for input.\n");
	getchar();
	return 0;
}



//4
int f4() {
	//read(); -> é suposto dar erro!
	return 0;
}



//5a)
int f5a(int argc, char* argv[], char* env[]) {
	printf("%d\n",argc);
	for(int i=0;i<argc;i++) 
		printf("%s\n",argv[i]);
	int i = 0;
	while (env[i] != NULL) {		
		printf("%s\n",env[i]);
		i +=1;
	}
	return 0;
}


//5b)
int f5b(int argc, char* argv[], char* env[]) {
	for(int j=0;j<=argc-1;j++) {
		int i = 0;
		while (env[i] != NULL) {
			char c, a[1000];
			memset(a,0,sizeof(a));
			int k=0;
			
			do {
				c = env[i][k];
				if (c!='=')
					a[k] = c;
				k++;
			} while (c != '=');

			if (!strcmp(argv[j],a)) {	
				printf("%s\n",env[i]);
				break;
			}
			i +=1;
		}
	}
	return 0;
}


//5c)
/*Para criar uma environment variable faz-se:
export DIR1="testing-123"
e depois executa-se o programa com ./fe1 DIR1*/
int f5c(int argc, char* argv[], char* env[]) {
	if (argc == 1) f5a(argc,argv,env);
	else f5b(argc,argv,env);
	return 0;
}



//6

int f6(int argc, char* argv[], char* env[]) {
	if (argc == 1) {
		printf("no arguments\n");
		return 0;
	}
	int fd = open(argv[1], O_RDONLY);
	printf("fd: %d\n",fd); // standard output (output.log)
	if (fd < 0) {
		printf("Error: %d\n" , errno); //a) standard output (output.log)
		fprintf(stderr, "Error: %d\n", errno); //b) standard error (error.log)
		perror("Error"); //c) standard error (error.log)
	}
	
	return 0;
}



//7
/*
Uma das funções main do OS é disponibilizar camada de abstraçao para não lidar com hardware (impressoras, etc)
segurança -> garantir que os nossos processos n façam "asneiras" na memoria, espaço, etc e garante a não mistura de processos
*/


//8
//all of them?



//9a)
//We can read the date but we can't use the write operations


//9b)
//date



//10
int f10a() { // exec with "time ./fe1"
	for (int i = 0; i < 100000; i++)
		printf("Hello world!\n");
	return 0;
}

/*struct tms {
	clock_t tms_utime; // tempo de CPU gasto em código do processo
	clock_t tms_stime; // tempo de CPU gasto em código do sistema chamado pelo processo
	clock_t tms_cutime; // tempo de CPU dos filhos (código próprio)
	clock_t tms_cstime; // tempo de CPU dos filhos (código do sistema)
}*/

int f10b() { // using <sys/times.h>
	clock_t start, end;
	struct tms t;
	int long ticks = sysconf(_SC_CLK_TCK);

	start = times(&t); /* início da medição de tempo */

	for (int i = 0; i < 100000; i++)
		printf("Hello world!\n");

	end = times(&t); 

	printf("Clock: %.5f sec\n",(double)(end - start) / ticks);
	printf("User time: %.5f sec\n",(double)(t.tms_utime)/ticks);
	printf("System time: %.5f sec\n",(double)(t.tms_stime)/ticks);
	return 0;
}



int f11a(int argc, char* argv[]) {
	if (argc != 3) {
		printf("2 files are needed!\n");
		return -1;
	}
	int f1 = open(argv[1],O_RDONLY);
	if (f1 < 0) {
		printf("1 file doesn't open!\n");
	}
	int f2 = open(argv[2],O_WRONLY);
	if (f2 < 0) {
		printf("2 file doesn't open!\n");
	}
	
	char buf[128];

	while(read(f1,buf,128) > 0) {
		write(f2,buf,strlen(buf));
		memset(buf,0,sizeof(buf));
	}
	return 0;
}

int f11b(int argc, char* argv[]) {
	char buf[256];
	if (argc != 3) {
		printf("2 files are needed!\n");
		return -1;
	}
	FILE *f1,*f2;
	f1 = fopen(argv[1],"r");
	if (f1 == NULL) {
		printf("1 file doesn't exist!\n");
		return -1;
	}
	f2 = fopen(argv[2],"w"); //não é preciso existir f2. para verificar abrir para ler o ficheiro e fechar.

	while((fgets( buf, 256, f1 ))!= NULL)
	{
		fputs(buf, f2);
	}

	return 0;

	/*int c;
	while(1) {
		c = fgetc(f1);
		if (feof(f1))
			break;
		fputc(c,f2);
	}*/
}

int f11c(int argc, char* argv[]) {
	//system calls
	if (argc == 2) {
			int f1 = open(argv[1],O_RDONLY);
		if (f1 < 0) {
			printf("1 file doesn't open!\n");
		}
		
		char buf[128];

		while(read(f1,buf,128) > 0) {
			printf("%s\n", buf);
			memset(buf,0,sizeof(buf));
		}
		return 0;
	}
	else if (argc != 3) {
		printf("2 files are needed!\n");
		return -1;
	}
	int f1 = open(argv[1],O_RDONLY);
	if (f1 < 0) {
		printf("1 file doesn't open!\n");
	}
	int f2 = open(argv[2],O_WRONLY);
	if (f2 < 0) {
		printf("2 file doesn't open!\n");
	}
	
	char buf[128];

	while(read(f1,buf,128) > 0) {
		write(f2,buf,strlen(buf));
		memset(buf,0,sizeof(buf));
	}
	return 0;
}



//12
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


/*struct dirent {
   	ino_t          d_ino;       // Inode number
 	off_t          d_off;       // Not an offset; see below
   	unsigned short d_reclen;    // Length of this record
 	unsigned char  d_type;      // Type of file; not supported by all filesystem types
   	char           d_name[256]; // Null-terminated filename
};*/

/*struct stat {
	mode_t st_mode; // tipo de ficheiro e permissões
	ino_t st_ino; // número do i-node correspondente
	nlink_t st_nlink; // contagem de links
	uid_t st_uid; // identificador do dono
	gid_t st_gid; // identificador do grupo do dono
	off_t st_size; // tamanho em bytes (ficheiros normais)
	time_t st_atime; // último acesso
	time_t st_mtime; // última modificação
	time_t st_ctime; // último mudança de estado
	long st_blocks;// número de blocos alocados
}*/

int f12a(int argc, char* argv[]) {
	if (argc != 2) {
		printf("You must give a directory!\n");
		return 1;
	}

	DIR * dir = opendir(argv[1]);

	if (dir == NULL) {
		printf("DIR doesn't exist!\n");
		return 1;
	}

	
	struct dirent* dirent = readdir(dir);

	struct stat st;

	while(dirent != NULL) {
		char* s = malloc(strlen(argv[1]) + strlen(dirent->d_name) +2);
		s = strcat(s,argv[1]);
		s = strcat(s,"/");
		s = strcat(s,dirent->d_name);
		stat(s,&st);
		if (S_ISREG(st.st_mode)) {
			printf("NAME: %s        SIZE: %ld bytes\n", dirent->d_name, st.st_size);
		}
		dirent = readdir(dir);
	}

	return 0;
}

//12c)
/* executar f12a() no terminal com './fe1 DIR > files.txt'
usando o wc para ver quantos ficheiros regulares temos na DIR fazer 'wc -l files.txt' (-l: number of lines)*/



int main(int argc, char* argv[], char* env[]) {
	//f3();
	//f4();
	//f5a(argc,argv,env);
	//f5b(argc,argv,env);
	//f5c(argc,argv,env);
	//f6(argc,argv,env);
	//f10a();
	//f10b();
	//f11a(argc,argv);
	f11b(argc,argv);
	//f11c(argc,argv);
	//f12a(argc,argv);
	return 0;
}