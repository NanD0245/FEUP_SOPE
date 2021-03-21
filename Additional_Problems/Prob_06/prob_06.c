#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdlib.h>


//1
#define STDERR 2
#define NUMITER1 10000

void * handler1(void * arg) {
	int i;
	int num = *(int *)arg;
	char str[10];
	fprintf(stderr, "Starting thread %d\n", num);
	sprintf(str,"%d",num);
	for (i = 1; i <= NUMITER1; i++) write(STDERR,str,1);
	return arg;
}

int f1() {
	pthread_t ta, tb;
	
	void *n1,*n2;

	int i = 1, j = 2;
	pthread_create(&ta, NULL, handler1, &i);
	pthread_create(&tb, NULL, handler1, &j);
	pthread_join(ta, &n1);
	pthread_join(tb, &n2);

	printf("\n%d\n", *(int *)n1);
	printf("\n%d\n", *(int *)n2);
	return 0;
} 



//2
#define NUMITER2 50000

int char_numbers;
void * handler2(void * arg) {
	int i,count = 0;
	char str[2];
	sprintf(str,"%d",*(int*)arg);
	fprintf(stderr, "Starting thread %d\n", *(int*)arg);
	for (i = 1; char_numbers > 0; i++) {count++;char_numbers--;write(STDERR,str,1);}
	*(int *)arg = count;
	return arg;
}

int f2() {
	char_numbers = NUMITER2;
	pthread_t ta, tb;
	void *n1,*n2;
	int a = 1,b = 2;
	pthread_create(&ta, NULL, handler2, &a);
	pthread_create(&tb, NULL, handler2, &b);
	pthread_join(ta, &n1);
	pthread_join(tb, &n2);
	printf("\n%d\n", *(int *)n1);
	printf("\n%d\n", *(int *)n2);
	return 0;
} 



//3
#define NUM_THREADS 10

void *PrintHello(void *threadnum) {
	printf("Hello from thread no. %d!\n", *(int *) threadnum);
	pthread_exit(NULL);
}

int f3() {
	pthread_t threads[NUM_THREADS];
	int args[NUM_THREADS];
	for(int t=0; t< NUM_THREADS; t++){
		args[t] = t;
		printf("Creating thread %d\n", t);
		pthread_create(&threads[t], NULL, PrintHello, (void *)&args[t]);
	}
	pthread_exit(0);
	//exit(0);
}

//a) existem erros na memória devido à sua partilha entre os threads
//c) com o exit() irão ocorrer erros nos prints



//4

int main() {
	return f3();
}