#include <iostream>
#include "projectPattern.h"
#include "pthread.h"

#include <queue>

#define NUM_THREADS 3

using namespace std;

struct argData {
	int start;
	int end;
	int threadCount;
};

//struct pipelineArgs {
//	pthread_mutex_t lock;
//	pthread_cond_t cond;
//	queue<int> input;
//	queue<int> output;
//};



void parallelPatterns::parallelFor(int numItems, void* (*func)(void*))
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

void parallelPatterns::parallelFor2D(int numRows, void* (*func)(void*))
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

void parallelPatterns::pipelineInit(void* (*func[])(void*)) 
{
	int functionCount = NUM_THREADS;
	pthread_t Threads[NUM_THREADS];
	//pipelineArgs args[NUM_THREADS];
	for (int i = 0; i < functionCount; i++) {
		/*queue<int> input;
		queue<int> output;
		args[i].input = input;
		args[i].output = output;
		args[i].cond = PTHREAD_COND_INITIALIZER;
		args[i].lock = PTHREAD_MUTEX_INITIALIZER;*/
		pthread_create(&Threads[i], NULL, func[i], NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}
