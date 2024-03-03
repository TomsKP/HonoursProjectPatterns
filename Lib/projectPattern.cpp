#include <iostream>
#include "projectPattern.h"
#include "pthread.h"

#define NUM_THREADS 3

using namespace std;

struct argData {
	int start;
	int end;
	int threadCount;
};


void parallelPatternFor::parallelFor(int numItems, void* (*func)(void*))
{
	int numOptions = numItems;
	int optPer = numOptions / NUM_THREADS;
	pthread_t Threads[NUM_THREADS];
	argData arguments[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS-1; i++) {
		arguments[i].start = i * optPer;
		arguments[i].end = (i + 1) * optPer;
		arguments[i].threadCount = NUM_THREADS;
		int status = pthread_create(&Threads[i], NULL, func, (void*) &arguments[i]);
		printf("Thread status: %d\n", status);
	}
	arguments[NUM_THREADS-1].start = (NUM_THREADS - 1) * optPer;
	arguments[NUM_THREADS-1].end = numOptions;
	int status = pthread_create(&Threads[NUM_THREADS-1], NULL, func, (void*) &arguments[NUM_THREADS-1]);
	printf("Thread status: %d\n", status);

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}

void parallelPatternFor::parallelFor2D(int numRows, void* (*func)(void*))
{
	int rows = numRows;
	int rowsPer = rows / NUM_THREADS;
	pthread_t Threads[NUM_THREADS];
	argData arguments[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS - 1; i++) {
		arguments[i].start = i * rowsPer;
		arguments[i].end = (i + 1) * rowsPer;
		arguments[i].threadCount = NUM_THREADS;
		int status = pthread_create(&Threads[i], NULL, func, (void*)&arguments[i]);
		printf("Thread status: %d\n", status);
	}
	arguments[NUM_THREADS - 1].start = (NUM_THREADS - 1) * rowsPer;
	arguments[NUM_THREADS - 1].end = rows;
	int status = pthread_create(&Threads[NUM_THREADS - 1], NULL, func, (void*)&arguments[NUM_THREADS - 1]);
	printf("Thread status: %d\n", status);

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}
