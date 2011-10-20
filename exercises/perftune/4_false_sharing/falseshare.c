/*

False sharing example (pthreads)

Andrzej Nowak
CERN

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define ITERATIONS 100000000

float *result_array;
float *trash_array;
pthread_t *pthread_array;

int NUM_THREADS = 0;

void *worker(void *data) {
    int i = 0;
    int thread_id = -1;
    
    for(i=0; i<NUM_THREADS; i++)
        if(pthread_array[i] == pthread_self())
            thread_id = i;
    
    for(i=0; i<ITERATIONS; i++) {
        result_array[thread_id] += trash_array[i];
//        result_array[thread_id] /= 2.11321432432;
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s #threads\n", argv[0]);
        exit(-1);
    }
    
    NUM_THREADS = atoi(argv[1]);
    result_array = (float *)calloc(NUM_THREADS, sizeof(float));
    trash_array = (float *)calloc(ITERATIONS, sizeof(float));
    pthread_array = (pthread_t *)calloc(NUM_THREADS, sizeof(pthread_t));

    int i = 0;

    for(i=0; i<NUM_THREADS; i++)
        pthread_create(&pthread_array[i], NULL, worker, NULL);
    
    for(i=0; i<NUM_THREADS; i++)
        pthread_join(pthread_array[i], NULL);
        
    for(i=0; i<NUM_THREADS; i++)
        printf("Result for thread %d: %f\n", i, result_array[i]);
    return 0;
}
