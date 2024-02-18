#include <iostream>
#include "projectPattern.h"
#include "pthread.h"

#define NUM_THREADS 4

using namespace std;

struct argData {
	int start;
	int end;
	int row;
};


void parallelPatternFor::parallelFor(int numData, void* (*func)(void*))
{
	int numOptions = numData;
	int optPer = numOptions / NUM_THREADS;
	pthread_t Threads[NUM_THREADS];
	argData arguments[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS-1; i++) {
		arguments[i].start = i * optPer;
		arguments[i].end = (i + 1) * optPer;
		arguments[i].row = i;
		int status = pthread_create(&Threads[i], NULL, func, (void*) &arguments[i]);
		printf("Thread status: %d\n", status);
	}
	arguments[NUM_THREADS-1].start = (NUM_THREADS - 1) * optPer;
	arguments[NUM_THREADS-1].end = numOptions;
	arguments[NUM_THREADS - 1].row = NUM_THREADS;
	int status = pthread_create(&Threads[NUM_THREADS-1], NULL, func, (void*) &arguments[NUM_THREADS-1]);
	printf("Thread status: %d\n", status);

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}
