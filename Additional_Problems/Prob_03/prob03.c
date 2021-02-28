#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

//1
int global=1;

int f1(void) {
	int local = 2;
	if(fork() > 0) {
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global++;
		local--;
	} else {
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global--;
		local++;
	}
	printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
	return 0;
}

/*
a)
Processo Pai: PID = 21321 ; PPID = 13755 ; global = 2 ; local = 1

Processo Filho: PID = 21322 ; PPID = 21321 ; global = 0 ; local = 3

b)
O PID e o PPID estão sempre a mudar de execução para execução.
A variável global e a local são constantes para o pai e para o filho.
*/


//2
int f2a(void) {
	write(STDOUT_FILENO,"1",1);
	if(fork() > 0) {
		write(STDOUT_FILENO,"2",1);
		write(STDOUT_FILENO,"3",1);
		//wait(NULL);
	} else {
		write(STDOUT_FILENO,"4",1);
		write(STDOUT_FILENO,"5",1);
	}
	write(STDOUT_FILENO,"\n",1);
	return 0;
} 

int f2c(void) {
	printf("1");
	if(fork() > 0) {
		printf("2");
		printf("3");
	} else {
		printf("4");
		printf("5");
	}
	printf("\n");
	return 0;
} 

/*
a)
Todos os números (1 a 5) podem ser escritos.

b)
123
45

c)
123
145

o "1" fica no buffer e é impresso duas vezes.

d)
com o '\n' depois do 1 o buffer é limpo antes do fork sendo só impresso uma vez
*/


//3
#define MAX 50000
int f3(void) {
	int i;
	pid_t pid;
	char str[10];
	pid=fork();
	switch (pid) {
		case -1:
			perror("fork");
			break;
		case 0: //filho
			for (i=1; i<=MAX; i++) {
				sprintf(str,"-%d",i);
				write(STDOUT_FILENO,str,strlen(str));
			}
			break;
		default: //pai
			for (i=1; i<=MAX; i++) {
				sprintf(str,"+%d",i);
				write(STDOUT_FILENO,str,strlen(str));
			}
			//break;
	}
	return 0;
} 

//Os processos pai e filho executam as instruções em sequências praticamente alternadas (tem algumas falhas)


//4
int f4a() {
	int id = fork();
	switch (id) {
		case 0:
			printf("Hello ");
			break;
		default:
			wait(NULL);
			printf("world !\n");

	}
	return 0;
}

int f4b() {
	int pid = fork();
	switch(pid) {
		case 0:
			printf("world !\n");
			break;
		default:
			printf("Hello ");
			fflush(stdout);
			wait(NULL);
	}
	return 0;
}

//neste caso o filho pode executar primeiro
//a unica maneira de o filho esperar pelo pai é usando signal


//5
int f5() {
	int pid2,pid = fork();
	if (pid == 0) printf("Hello ");
	else {
		wait(NULL);
		pid2 = fork();
		if (pid2 == 0) printf("my ");
		else {
			wait(NULL);
			printf("friends!\n");
		}
	}
	return 0;
}



//6
int f6(void)
{
	pid_t pid;
	int i, j;
	printf("I'm process %d. My parent is %d.\n", getpid(),getppid());
	//pid = fork();
	for (i=1; i<=3; i++) {
		pid = fork();
		if ( pid < 0) {
			printf("fork error");
			exit(1);
		}
		else if (pid == 0) {
			printf("I'm process %d. My parent is %d.I'm going to work for 1 second ...\n",getpid(),getppid());
			//sleep(1); // simulando o trabalho do filho
			printf("I'm process %d. My parent is %d.I finished my work\n", getpid(),getppid());
			//exit(0); // a eliminar na alinea c)
		}
		else {// simulando o trabalho do pai
			for (j=1; j<=5; j++) {
				waitpid(pid,NULL,WNOHANG);
				//sleep(1);
				printf("father working ...\n");
			}
		}
	}
	exit(0);
} 

/*
a) aparece mais um processo zombie a cada 10 segundos (status -> Z+)

c) vão existir 7 "filhos" e 8 "pais" ao longo do for loop o que vai resultar 
de 7 conjuntos de prints para os filhos e 7 conjuntos de prints para osfilhos
*/



//7
int f7(int argc, char *argv[]) {
	char prog[20];
	sprintf(prog,"%s.c",argv[1]);
	execlp("gcc","gcc",prog,"-Wall","-o",argv[1],NULL);
	printf("não executou com sucesso\n");
	exit(1);
} 

//compila o ficheiro dado como argumento (nome sem ".c")


//8
int f8a(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		execlp("ls", "ls", "-laR", argv[1], NULL);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}

int f8b(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		execl("/usr/bin/ls", "ls", "-laR", argv[1], NULL);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}

int f8c(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		char* args[] = {"ls", "-laR", argv[1], NULL};
		execvp("ls", args);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}

int f8d(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		char* args[] = {"ls", "-laR", argv[1], NULL};
		execv("/usr/bin/ls", args);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}

int f8e(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		char* args[] = {"ls", "-laR", argv[1], NULL};
		execve("/usr/bin/ls", args, envp);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}



//9
int f9(int argc, char *argv[], char *envp[]) {
	pid_t pid;
	int status;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();

	if (pid > 0) {
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
		wait(&status);
	}
	else if (pid == 0){
		//printf("PID: %d\n", getpid()); //obter pid do child para dar kill
		//sleep(5);
		char* args[] = {"ls", "-laR", argv[1], NULL};
		execvp("ls", args);
		printf("Command not executed !\n");
		exit(1);
	}
	printf("\nExit code of child process: %d\n", status);
	if (WIFEXITED(status)) //se dir não existe entra aqui. devia???
		printf("Ended normally! :)\n");
	else if (WIFSIGNALED(status))
		printf("Ended anormally! :(\n");

	exit(0);
}

/*
a)
Existing directory: 0
Nonexistent directory: 512
By using kill [PID]: 15

b)
existent dir -> ended normally
non-existent dir -> ended normally
*/



//10
int f10(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (!(argc == 2 || argc ==3)) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	if (argc == 2) {return f8c(argc,argv,envp);}

	pid=fork();
	if (pid > 0)
		printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		int file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
		dup2(file,1);
		char* args[] = {"ls", "-laR", argv[1], NULL}; //não dá para usar ls -laR > argv[1] ???
		execvp("ls", args);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
}


int main(int argc, char *argv[], char *envp[]) {
	//f1();
	//f2a();
	//f2c();
	//f3();
	//f4a();
	//f4b();
	//f5();
	//f6();
	//f7(argc,argv);
	//f8a(argc,argv,envp);
	//f8b(argc,argv,envp);
	//f8c(argc,argv,envp);
	//f8d(argc,argv,envp);
	//f8e(argc,argv,envp);
	//f9(argc,argv,envp);
	f10(argc,argv,envp);
	return 0;
}


