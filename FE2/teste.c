#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

#define NUM_THREADS 10 

void *func_thread(void *num) {    
    sleep(1);
    int* n = (int*)num; 
    printf("%d\n", *n);
    return num;
} 

int main() {    
    pthread_t threads[NUM_THREADS];    
    int t;    

    int args[NUM_THREADS];
    void* num;
    for(t=0; t< NUM_THREADS; t++){ 
        args[t] = t;
        printf("Creating thread %d\n", t);       
        pthread_create(&threads[t], NULL, func_thread, (void *)&args[t]);  
        //pthread_join(threads[t], &num);
    }    
    
    for(t = 0; t < NUM_THREADS; t++){
        pthread_join(threads[t], &num);
        printf("Main Thread: %d\n", *(int*)num);
    }
    
    return 0;
}