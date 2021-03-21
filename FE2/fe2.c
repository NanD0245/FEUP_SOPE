#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


//1
//Program: is an executable file containing the set of instructions written to perform a specific job on your computer.

//Process: is an executing instance of a program and have independent memory

//Thread:  is the smallest executable unit of a process and can access memory of other thread



/*2
a)
PID - Process IDentifier
PPID - Parent Process IDentifier
real U/GID - User/Group IDentifier of who initiated the process
current dir - directory to where names of files are referenced by default
file descriptor table - info on open files; descriptor is table index
environment - initially inherited from parent process
text space - memory where program instructions lie; read only
stack space - memory automatically managed
heap space - memory managed by the user in runtime
priority - info for process scheduling
signal disposition - masks for delivery or blocking of signals
umask - mask that restrains files' permissions on creation

b)
Program counter
Registers
Stack
State


c)
Per process items:							Per thread items:

Address space								Program counter
Global variables							Registers
Open files									Stack
Child processes								State
Pending allarms
Signals and signal handlers
Accounting information
*/



/*3
a)

S -> interruptible sleep (waiting for an event to complete)
s -> is a session leader
I -> Idle kernel thread
< -> high-priority
N -> low-priority
l -> multi-threaded
+ -> is in the foreground process group
R -> running or runnable

b)initial -> bash -> ps

c)systemd -> yakuake -> bash -> pstree
*/



/*4
New -> Running
Running -> Blocked
Blocked -> Ready
Running -> Ready
Ready -> Running
Ready -> Terminated
*/


//5
int f5() {
	int id = fork();

	wait(NULL); //parent wait for child
	
	if (id != 0) //parent
		printf("World\n");

	switch(id) {
		case 0:
			printf("Hello "); //child
			break;
	}

	return 0;
}

int f5reverse() {
	//fflush(stdout);
	int id = fork();
	switch(id) {
		case 0:
			printf("World!\n");
			break;
		default:
			printf("Hello ");
	}
	return 0;
}



//6
//Based on Processes: each process cant access the memory of another process so there will be a lot of less errors
//Based on Threads: each thread can access the memory of another thread but this easily can create some memory errors



//7.
//Asynchornous communication


//8
void func(int signo) {
	fprintf(stderr,"Hello!\n");
}

int f8() {
	struct sigaction new, old;
	sigset_t smask;	// defines signals to block while func() is running

	// prepare struct sigaction
	if (sigemptyset(&smask)==-1)	// block no signal
		perror ("sigsetfunctions");

	new.sa_handler = func;
	new.sa_mask = smask;
	new.sa_flags = 0;	// usually works

	if(sigaction(SIGUSR1, &new, &old) == -1)
				perror ("sigaction");

	pid_t pid = fork();
	switch (pid) {
		case -1:
			perror("fork failed");
			return -1;
		case 0:
			sleep(1);
			break;
		default:
			fprintf(stderr,"World: ");
			kill(pid,SIGUSR1);
			wait(NULL);
			break;
	}
	return 0;
}



//9
#define NTHREADS 3

void *rot(void *a) {
	printf("\ni = %d",*(int*)a);
	*(int*)a *= *(int*)a; 
	printf("\n\t In thread PID: %d ; TID: %lu.", getpid(), (unsigned long) pthread_self());
	sleep(1);
	return a;
}

int f9() {
	int i; // thread counter
	pthread_t ids[NTHREADS];	// storage of (system) Thread Identifiers


	setbuf (stdout, NULL);	// only for debugging
	printf("\nMain thread PID: %d ; TID: %lu.\n", getpid(), (unsigned long) pthread_self());

	int args[NTHREADS];
	
	// new threads creation
	for(i=0; i<NTHREADS; i++) {
		args[i] = i;
		if (pthread_create(&ids[i], NULL, rot, (void*)&args[i]) != 0)
			exit(-1);	// here, we decided to end process
		else
			printf("\nNew thread %d ; TID: %lu.", i, (unsigned long) ids[i]);
	}

	// wait for finishing of created threads	
	void *num;
	for(i=0; i<NTHREADS; i++) {
		pthread_join(ids[i], &num);	// Note: threads give no termination code
		printf("\nTermination of thread %d: %lu.", i, (unsigned long)ids[i]);
		printf("\ni * i = %d\n", *(int *)num);
	}

	printf("\n");
	pthread_exit(NULL);	// here, not really necessary...
	return 0;	// will not run this!
}

//b) comando: ps -eLf



//10
struct Arg
{
	char* s1;
	char* s2;
	int p;
};

void * print(void * arg) {
	struct Arg *argg = (struct Arg *)arg;
	
	//printf("%d\n", argg->p);
	if (argg->p == 0) {
		printf("%s", argg->s1);
		argg->p = 1;
	}
	else if (argg->p == 1) {
		printf("%s", argg->s2);
	}
	arg = (void*)argg;
	return arg;
}

int f10() {
	struct Arg *args;
	pthread_t ids[2];
	args = malloc(sizeof(struct Arg));
	args->p = 0;
	args->s1 = "Operating ";
	args->s2 = "Systems!\n";

	for (int i = 0; i < 2; i++) {
		if (i == 1) sleep(1);
		if (pthread_create(&ids[i],NULL,print,(void*)args) != 0)
			exit(-1);	// here, we decided to end process
	}

	for (int i = 0; i < 2; i++) { //how to wait???
		pthread_join(ids[i],NULL);
	}
	return 0;
}



//11
/*
PIPE 													FIFO

-PIPES doesn’t exist in the filesystem.					-FIFO exists in the filesystem.
-PIPES are unidirectional.								-FIFO are bi-directional same fifo can be used for reading and writing.
*/


//12
/*

In a named pipe, the program described could be executed between unrelated processes to verify its premiss. 
As such, two named pipes would be set up between both, one representing the color to send and to process by 
the reading process, and one for the answer. The program would be as follows: the first process would write 
to the first pipe the color, and the second process would read it, process it, and put its response on the 
second pipe, which would be later read by the first process.

Using normal pipes, this program could only be implemented between child and parent processes. As such, the 
process could only verify that it had the color on its private pallete, using the corresponding pipe through 
a child co-process. This could not be extended to other processes using normal pipes as they don't allow 
unrelated process comunication.

It is impossible to execute the program described using signals since they do not allow to send information. 
Since there is no way to inform the second process what color he wants to refer to, the second process cannot 
form a valid response and the premisse of the program fails.

*/


//13
int f13a(){
    int   n, fd[2];
    pid_t pid;
    char  line[10];
    if (pipe(fd) < 0) {
        perror("ERROR");
        exit(1);
    } 
    if ( (pid = fork()) < 0) {
        perror("ERROR");
        exit(1);
    } 
    else if (pid > 0) {
        close(fd[0]); 
        write(fd[1], "Operating", 9);
    } 
    else {                      
        close(fd[1]);           
        n = read(fd[0], line, 10);
        char* st = malloc(sizeof(char) * n);
        memcpy(st, line, n);
        printf("%s Systems", st);
        free(st);
    }
    return 0;
}

int f13b(){
    int   n;
    pid_t pid;
    char  line[10];
    if (mkfifo("p1", 0666) < 0) {
       	perror("ERROR");
        exit(1);
    } 
    if ( (pid = fork()) < 0) {
        perror("ERROR");
        exit(1);
    } 
    else if (pid > 0) {
    	int p1;
    	while ((p1 = open("p1", O_WRONLY)) < 0);
        write(p1, "Operating", 9);
        close(p1);
    } 
    else {                      
        int p1;
    	while ((p1 = open("p1", O_RDONLY)) < 0);
    	n = read(p1, line, 10);
        char* st = malloc(sizeof(char) * n);
        memcpy(st, line, n);
        printf("%s Systems", st);
        free(st);
        close(p1);
    }
    return 0;
}

//f13c & f13d semelhante a f13a e f13b

int main() {
	return f10();
}

//Hello Hello World!
