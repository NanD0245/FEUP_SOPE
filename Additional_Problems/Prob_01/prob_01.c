#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

/*1a)

cd -> enter ina a directory
ls -> shows content of a directory
mkdir -> creates a directory
rmdir -> remove a directory
cp -> copy content of a file to another file
mv -> rename of move files
cat -> shows content of a file and concatenate files
more -> view file in terminal
tail -> print the last 10 lines of a file
pwd -> shows de path to a directory
ps -> shows the current processes
find -> show all the files inside a directory hierarchy
grep -> print lines that matches a pattern
chmod -> change the file mode
clear -> clear terminal
echo -> shows a string into standard output (terminal)

b) ls ; count && ls | count */



//2)
int f2a() {
	printf("Hello World!\n");
	return 0;
}

//b) must have a function named main



//3a) echo $? shows a '0' (shows the return code of the last executed command)

//b) instead o a '0' shows a '2'

int f3b() {
	printf("Hello World!\n");
	return 2;
}

//c) ls returns the code '0'



//4
int f4a(int argc, char* argv[]) { //use ./prob_o1 'More than 1 word'
	if (argc == 1 || argc > 2) {
		printf("You have to pass only on argument with a name\n");
		return -1;
	}
	printf("Hello %s\n", argv[1]);
	return 0;
}

int f4b(int argc, char* argv[]) { //use ./prob_o1 'More than 1 word'
	if (argc < 3 || argc > 3) {
		printf("You have to pass one argument with a name and other with a number\n");
		return -1;
	}
	for (int i = 0; i < atoi(argv[2]); i++) {
		printf("Hello %s\n", argv[1]);
	}
	return 0;
}



//5
int f5a(int argc, char* argv[], char* envp[]) {
	int i=0;
	while (envp[i] != NULL)
		printf("%s\n", envp[i++]);
	return 0;
}

int f5b(int argc, char* argv[], char* envp[]) {
	int i = -1;
	while (envp[++i] != NULL) {
		if (strncmp(envp[i],"USER=",5) == 0) 
			printf("Hello %s!\n", &envp[i][5]);
	}
	return 0;
}

int f5c() {
	printf("Hello %s!\n", getenv("USER"));
	return 0;
}

int f5d() { //after export USER_NAME='Fernando Luís Santos Rego'
	printf("Hello %s!\n", getenv("USER_NAME"));
	return 0;
}



//6
#define BUF_LENGTH 256
int f6(int argc, char* argv[])
{
	if (argc != 3) { //h)
		printf("usage: nome_do_executável file1 file2\n");
		return -1;
	}
	FILE *src, *dst;
	char buf[BUF_LENGTH];
	if ((src = fopen(/*"infile.txt" g)*/ argv[1], "r")) == NULL)
	{
		//printf("ERROR:: %s\n",strerror(errno)); //d)
		//perror("ERROR"); //d)
		//printf("%d\n", errno); //e)
		exit(1);
	}
	if ((dst = fopen(/*"outfile.txt" g)*/ argv[2], "w" )) == NULL)
	{
		exit(2);
	}
	while((fgets( buf, BUF_LENGTH, src ))!= NULL)
	{
		fputs(buf, dst);
	}
	fclose(src);
	fclose(dst);
	exit(0); // zero é geralmente indicativo de "sucesso"
}

//b) o ficheiro infile.txt não existe!
//c) exit code = 1
//e) error code: 2 -> No such file or directory
//f) sim! copiou tudo para o outfile.txt
//g) quando não são passados argumentos nada é feito



//7
void handler1() {printf("Executing exit handler 1\n");exit(0);}

void handler2() {printf("Executing exit handler 2\n");exit(0);}

int f7a() {
	atexit(handler1);
	atexit(handler2);
	//abort();
	printf("Main done!\n");
	exit(0);
}

//b)A execução dos handlers ocorre quando o programa termina normalmente, portanto "Main done!" é mostrado primeiro
//e só depois os handlers por ordem inversa.
//Ao fazer uma chamada abort() antes de escrever "Main done!" nada é mostrada pois o programa não termina normalmente,
//logo os handlers não são executados
//Ao terminar com exit() nada vai acontecer pois o programa termina normalmente



//8
int f8a(int argc, char* argv[]) {
	if (argc != 3) {
		printf("usage: nome_do_executável n1 n2\n");
		return -1;
	}
	int random, n1 = atoi(argv[1]), n2 = atoi(argv[2]);
	time_t t;
	srand((unsigned)time(&t));
	do {
		random = rand() % n1;
		printf("%d\n",random);
	} while (random != n2);
	return 0;
}

//b) executar com 'time ./f8a 1000 13' ou
int f8b(int argc, char* argv[]) {
	clock_t start,end;
	struct tms tms;
	start = times(&tms);
	long ticks = sysconf(_SC_CLK_TCK);

	if (argc != 3) {
		printf("usage: nome_do_executável n1 n2\n");
		return -1;
	}
	int random, n1 = atoi(argv[1]), n2 = atoi(argv[2]);
	time_t t;
	srand((unsigned)time(&t));
	do {
		random = rand() % n1;
		printf("%d\n",random);
	} while (random != n2);

	end = times(&tms);

	printf("Real time: %.10fs\n", (double)(end-start)/ticks);
	printf("User time: %.10fs\n", (double)(tms.tms_utime)/ticks);
	printf("System time: %.10fs\n", (double)(tms.tms_stime)/ticks);
	return 0;
}



//9
int f9() {
	printf("script1 success!\n");
	return 0;
}



//10 command from argv
int f10a(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: nome_do_executável 'command'\n");
		return -1;
	}
	char* command = argv[1];
	char s[2] = " ";
	char* token = strtok(command, s);
	printf("%s\n", token);
	while((token = strtok(NULL, s)) != NULL)
		printf("%s\n", token);
	return 0;
}

int f10b(int argc, char* argv[]) {
	if (argc != 3) {
		printf("usage: nome_do_executável 'command' number_of_tokens\n");
		return -1;
	}
	char* tokens[atoi(argv[2])];
	char* command = argv[1];
	char s[2] = " ";
	char* token = strtok(command, s);
	tokens[0] = token;
	int i = 1;
	while((token = strtok(NULL, s)) != NULL) {
		tokens[i] = token;
		i++;
	}
	for (int i = 0; i < atoi(argv[2]); i++) {
		printf("%s\n", tokens[i]);
	}
	return 0;
}

int f10c(int argc, char* argv[]) {
	if (argc != 3) {
		printf("usage: nome_do_executável 'command' number_of_tokens\n");
		return -1;
	}
	char* tokens[atoi(argv[2])];
	char* command = argv[1];
	char s[3] = ";|";
	char* token = strtok(command, s);
	tokens[0] = token;
	int i = 1;
	while((token = strtok(NULL, s)) != NULL) {
		tokens[i] = &token[1];
		i++;
	}
	for (int i = 0; i < atoi(argv[2]); i++) {
		printf("%s\n", tokens[i]);
	}
	return 0;
}

int main(int argc, char* argv[], char* envp[]) {
	//return f2a();
	//return f3b();
	//return f4a(argc, argv);
	//return f4b(argc,argv);
	//return f5a(argc,argv,envp);
	//return f5b(argc,argv,envp);
	//return f5d();
	//return f6(argc,argv);
	//return f7a();
	//return f8a(argc,argv);
	//return f9();
	//return f10a(argc,argv);
	//return f10b(argc,argv);
	return f10c(argc,argv);
}