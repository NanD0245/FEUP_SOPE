#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>


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



//8
void func(int signo) {
	//fprintf(stderr, "\nReceived signal %d!\n", signo);
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
			printf("Hello ");
			kill(getppid(),SIGUSR1);
			break;
		default:
			pause();
			printf("World!\n");
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

int f9a() {
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
		if (pthread_create(&ids[i],NULL,print,(void*)args) != 0)
			exit(-1);	// here, we decided to end process
		//sleep(1);
		pthread_join(ids[i],NULL);
	}

	/*for (int i = 0; i < 2; i++) { //how to wait???
		pthread_join(ids[i],NULL);
	}*/
	return 0;
}

int main() {
	return f10();
}

//Hello Hello World!
