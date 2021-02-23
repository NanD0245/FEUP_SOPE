#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


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
Ready -> New
New -> Running
Running -> Blocked
Running -> Ready
Blocked -> Ready
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
	printf("Hello ");
	fflush(stdout);
	int id = fork();
	switch(id) {
		case 0:
			printf("World!\n");
			break;
	}
	return 0;
}



//6
//Based on Processes: each process cant access the memory of another process so there will be a lot of less errors
//Based on Threads: each thread can access the memory of another thread but this easily can create some memory errors



//7
 

int main() {
	return f6();
}

//Hello Hello World!
