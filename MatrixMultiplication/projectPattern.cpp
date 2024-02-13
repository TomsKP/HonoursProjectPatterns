#include <iostream>
#include "projectPattern.h"
#include "pthread.h"

#define NUM_THREADS 4

using namespace std;


void parallelPatternFor::parallelFor(int numData, void* (*func)(void*), int *arg)
{
	int numOptions = numData;
	int optPer = numOptions / NUM_THREADS;
	pthread_t Threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS-1; i++) {
		arg[i * 2] = i * optPer;
		arg[i*2+1] = (i + 1) * optPer;
		int status = pthread_create(&Threads[i], NULL, func, (void*) i);
		printf("Thread status: %d\n", status);
	}
	arg[(NUM_THREADS-1)*2] = (NUM_THREADS - 1) * optPer;
	arg[((NUM_THREADS-1)*2)+1] = numOptions;
	int status = pthread_create(&Threads[NUM_THREADS-1], NULL, func, (void*) (NUM_THREADS-1));
	printf("Thread status: %d\n", status);

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}
