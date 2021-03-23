#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h> 

int f1(int argc, char* argv[]) {
	if (argc != 2) {
		printf("You must give a directory!\n");
		return 1;
	}

	DIR * dir = opendir(argv[1]);

	if (dir == NULL) {
		printf("DIR doesn't exist!\n");
		return 1;
	}

	
	struct dirent* dirent;

	struct stat st;

    if (argv[argc - 1][strlen(argv[argc - 1]) - 1] != '/') {
        argv[argc - 1][strlen(argv[argc - 1]) + 1] = '\0';
        argv[argc - 1][strlen(argv[argc - 1])] = '/';
    }

	while((dirent = readdir(dir)) != NULL) {
        if (!strcmp(dirent->d_name, "..")) continue;
        if (!strcmp(dirent->d_name, ".")) continue;
		char* s = malloc(strlen(argv[1]) + strlen(dirent->d_name) +2);

		s = strcat(s,argv[1]);
		s = strcat(s,dirent->d_name);
		stat(s,&st);
		if (S_ISREG(st.st_mode)) {
			printf("NAME: %s        SIZE: %ld bytes\n", s, st.st_size);
		}
        if ( (st.st_mode)) {
            int pid = fork();
            int status;
            if (pid == 0) { //child
                argv[argc - 1] = s;
                execv(argv[0],argv);

                exit(0);
            }
            else waitpid(pid,&status,0);
        }
	}
	return 0;
}


// preparation for execve:
char *cmdline[] = { "sleep", "30", (void *)0 };
char *newenviron[] = { "USER=Linus", NULL };

int sexec() {
	pid_t id;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            if (execve("/bin/sleep", cmdline, newenviron) == -1)
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
        }
    return 0;
}

char *cmdline1[] = {};

int f2() {
    pid_t id;

    int ret;
    setbuf(stdout, NULL);	// to make sure printf() have no buffer so, nodelay!

    printf("\nParent: %d. USER=%s", getpid(), getenv("USER"));

    id = fork();
    switch (id) {
        case -1:
            perror ("fork()");
            exit (1);
        case 0:	// child
            printf("\nChild: %d. USER=%s", getpid(), getenv("USER"));
            printf("\nChild: check that following exec was succeful with: \"ps x | grep sleep\"\n");
            int fd;
            if ((fd = open("ex2c", O_RDONLY)) == -1) //c) /bin/sleep -> ex2c
			    return 1;
            if (fexecve(fd, cmdline, newenviron) == -1) //a) execve -> fexecve
                perror("execve");
            printf("\nYou will not see this unless execve() fails! Know why?...\n");
            break;	// if execve fails...
        default:
            printf("\nParent still: %d. USER=%s", getpid(), getenv("USER"));
            wait(&ret); //b) wait
            printf("Return code: %d\n", WEXITSTATUS(ret));
        }

    return 0;
}

void sigintHandler(int sig) {
    printf("Entrei SIGINT Handler\n");
}

void sigusrHandler(int sig) {
    printf("Entrei SIGUSR1 Handler %d\n", getpid());
}


int f3() {
    printf("%d\n",getpid());

    signal(SIGINT, sigintHandler);
    signal(SIGUSR1, sigusrHandler);

    int id, pid = fork();

    switch (pid) {
        case 0:
            id = fork();
            switch(id) {
                case 0:
                    sleep(5);
                    break;

                default:
                    wait(NULL);
                    break;
            }
            break;

        default:
            wait(NULL);
    }

    printf("Saí processo %d\n", getpid());

    return 0;
}

//a) all processes are in the same group

//b) just processe leader recieve the signal

//c) all processes recieve Ctrl-C signal


/*
4.
Mutexes -> locking mechanism and can have multiple program threads but not simultaneously. A mutex object is locked or unlocked

Semaphores -> signaling mechanism and can have multiple program threads simultaneously. Semaphore value is modified using wait() and signal()
*/



/*
5.
    int i;     // process number: 0 or 1
    int turn;  // shared variable for access to critical region

    enter_region(i)
        { while (turn != i) ; } // wait for turn
    leave_region(i)
        { turn = 1-i; }         // give turn away

The problem of this solution is that it is a Busy Waiting solution so it always spend the CPU continuously waiting to enter 
in critical region instead of simply blocking it.
On the other hand, this solution is correct because it prevents that two processes from being in the same critical region.
*/


/*
6.
    void fsin() { ... }

    int main() {
        struct sigaction ss;
        ... // set up ss with handler fsin();
        sigaction(SIGUSR1, &ss, NULL);

        int f = fork();
        switch (f) {
            case -1: perror("fork"); exit(1);
            case 0:  pause(); printf("hello!"); break;
            default: printf("World: ”); kill(f, SIGUSR1);
        }
    } // main()
    
    The child process wait forever when the father process send the SIGUSR1 signal before the child process makes pause();
*/


//7 -> see count.c

//a) Problem: some numbers are being repeated

#define MAXCOUNT 100000
long count;
int i = 0, turn =0;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

void *thr_fun(void *arg) {
	while (count < MAXCOUNT) {
		pthread_mutex_lock(&mut);
		printf("\n%ld",count);
		count++;
		pthread_mutex_unlock(&mut);
	}
	printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
	return (NULL);
}

int f7b(int argc, char *argv[]) {
	
	pthread_t *ptid;
	int nthreads;
	int i;

	setbuf(stdout,NULL);

	if (argc < 2) {
		printf("\nUsage: %s #threads\n", argv[0]);
		exit(1);
	}
	nthreads = atoi(argv[1]);
	ptid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
	if (ptid == NULL) {
		perror("malloc");
		exit(2);
	}
	for (i = 0; i< nthreads; i++) {
		if(pthread_create(&ptid[i],NULL,thr_fun,NULL) != 0) {
			perror("pthread_create");
			exit(3);
		}
	}
	for (i = 0; i< nthreads; i++)
		pthread_join(ptid[i],NULL);

	printf("\nEND!\n");
	return 0;
}


/*
8.
    #define N 5       // number of philosophers

    void philosopher(int i) {
        while (TRUE) {
            think();                // philosopher is thinking
            take_fork(i);           //take left fork
            take_fork((i+1) % N);   // take right fork
            eat();                  // yum-yum, spaghetti
            put_fork(i);            // put left fork back on the table
            put_fork((i+1) % N);    // put right fork back on the table
        }
    } //philosopher

    i - deadlock: The processes are waiting for each other and the waiting never ends.

    ii - livelock: for each philosopher (process) continually repeat the same interaction in response to changes in the 
    other processes without doing any useful work.
*/


/*
9.
    over-booking: just one process at a time

    deadlock: when a process (costumer) arrive the leader process (barber) wake up  ???? 
*/

/*
10.
    writer/reader starvation: if we prioritize one over the other will cause starvation on process with less priority!

    low throughput: the fact that reader and writer can't work at the same time it will cause a low throughput
*/


#define BUF_SIZE 20
#define MAX_ITER 100000
long /*count,*/ iter;
//int i = 0, turn =0;

//pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2=PTHREAD_MUTEX_INITIALIZER;

void *thr_fun_increase_m(void *arg) {
	while (iter < MAX_ITER) {
		pthread_mutex_lock(&mut);
		printf("\n%ld",count);
	    iter++;
        count++;
		pthread_mutex_unlock(&mut);
	}
	printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
	return (NULL);
}

void *thr_fun_decrease_m(void *arg) {
    printf("ENTREII");
    while (iter < MAX_ITER) {
		pthread_mutex_lock(&mut);
		printf("\n%ld",count);
		count--;
        iter++;
		pthread_mutex_unlock(&mut);
	}
	printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
	return (NULL);
}

int f11(int argc, char* argv[]) {
    pthread_t *ptid;
	int nthreads;
	int i;

	setbuf(stdout,NULL);

	if (argc < 2) {
		printf("\nUsage: %s #threads\n", argv[0]);
		exit(1);
	}
	nthreads = atoi(argv[1]);
	ptid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
	if (ptid == NULL) {
		perror("malloc");
		exit(2);
	}
	for (i = 0; i< nthreads; i++) {
		if( i%2 == 0 && pthread_create(&ptid[i],NULL,thr_fun_increase_m,NULL) != 0 ) {
			perror("pthread_increase_create");
			exit(3);
		}
        else if (i%2 == 1 && pthread_create(&ptid[i],NULL,thr_fun_decrease_m,NULL) != 0) {
            perror("pthread_decrease_create");
			exit(3);
        }
	}
	for (i = 0; i< nthreads; i++)
		pthread_join(ptid[i],NULL);

	printf("\nEND!\n");
	return 0;
}


sem_t sem;


void *thr_fun_decrease_s(void *arg) {
	while (iter < MAX_ITER) {
        sem_wait(&sem);
		printf("\n%ld",count);
        iter++;
		count--;
        sem_post(&sem);
	}
	printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
	return (NULL);
}

void *thr_fun_increase_s(void *arg) {
	while (iter < MAX_ITER) {
        sem_wait(&sem);
		printf("\n%ld",count);
	    iter++;
        count++;
        sem_post(&sem);
	}
	printf("\nEND thread %lu!\n", (unsigned long) pthread_self());
	return (NULL);
}

int f12(int argc, char *argv[]) {
	
	pthread_t *ptid;
	int nthreads;
	int i;

    sem_init(&sem, 0, 1); 

	setbuf(stdout,NULL);

	if (argc < 2) {
		printf("\nUsage: %s #threads\n", argv[0]);
		exit(1);
	}

	nthreads = atoi(argv[1]);

    printf("NUM THREADS: %d\n",nthreads);
    sleep(2);

	ptid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));

	if (ptid == NULL) {
		perror("malloc");
		exit(2);
	}
	for (i = 0; i < nthreads; i++) {
		if(i % 2 == 0 && pthread_create(&ptid[i],NULL,thr_fun_increase_s,NULL) != 0) {
			perror("pthread_create");
			exit(3);
		}
        else if (i % 2 == 1 && pthread_create(&ptid[i],NULL,thr_fun_decrease_m,NULL) != 0) {
			perror("pthread_create");
			exit(3);
		}
	}

    sleep(2);
    printf("NUM THREADS: %d\n",nthreads);
    sleep(2);

	for (i = 0; i < nthreads; i++)
		pthread_join(ptid[i],NULL);

    sem_destroy(&sem);

	printf("\nEND!\n");
	return 0;
}

int main(int argc, char* argv[], char* envp[]) {
    //return f1(argc,argv);
    //return f2();
    //return f3();
    //return f7b(argc,argv);
    //return f11(argc,argv);
    return f12(argc,argv);
}

