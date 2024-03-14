#include <iostream>
#include "pthread.h"
#include <queue>
#include "projectPattern.h"




#define NUM_THREADS 3

using namespace std;

struct argData {
	int start;
	int end;
	int threadCount;
};

struct pipelineArgs {
	pthread_mutex_t inputLock;
	pthread_cond_t inputCond;
	pthread_mutex_t outputLock;
	pthread_cond_t outputCond;
	queue<int> input;
	queue<int> output;
	queue<int>* inputptr;
	int (*func)(int);
	int length;
	bool finished = false;
};

void* pipelineStage(void* ptr);



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
	for (int i = 0; i < functionCount; i++) {
		pthread_create(&Threads[i], NULL, func[i], NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(Threads[i], NULL);
	}
}

void parallelPatterns::pipelineMain(int (*func[])(int), queue<int> &start, queue<int> &finalQueue, int length) {
	pthread_t Threads[NUM_THREADS];
	parallelPatterns pattern;
	queue<int> originalQueue = start;
	int queueSize = length;
	pipelineArgs args[3];

	for (int i = 0; i < NUM_THREADS; i++) {
		if (i == 0) {
			args[i].input = originalQueue;
			args[i].length = queueSize;
			args[i].inputptr = &args[i].input;
			args[i + 1].inputptr = &args[i].output;
			pthread_mutex_init(&args[i].inputLock, NULL);
			pthread_mutex_init(&args[i].outputLock, NULL);
			pthread_cond_init(&args[i].inputCond, NULL);
			pthread_cond_init(&args[i].outputCond, NULL);
			args[i + 1].inputLock = args[i].outputLock;
			args[i + 1].inputCond = args[i].outputCond;
		}
		else if (i == (NUM_THREADS - 1)) {
			args[i].length = queueSize;
			args[i].output = finalQueue;
			pthread_mutex_init(&args[i].outputLock, NULL);
			pthread_cond_init(&args[i].outputCond, NULL);
		}
		else {
			args[i + 1].inputptr = &args[i].output;
			args[i].length = queueSize;
			pthread_mutex_init(&args[i].outputLock, NULL);
			pthread_cond_init(&args[i].outputCond, NULL);
			args[i + 1].inputLock = args[i].outputLock;
			args[i + 1].inputCond = args[i].outputCond;
		}
		args[i].func = func[i];
	}

	for (int i = 0; i < 3; i++) {
		pthread_create(&Threads[i], NULL, pipelineStage, (void*)&args[i]);
	}

	for (int i = 0; i < 3; i++) {
		pthread_join(Threads[i], NULL);
	}

	for (int i = 0; i < length; i++) {
		cout << args[NUM_THREADS-1].output.front() << " " << endl;
		args[NUM_THREADS-1].output.pop();
	}
}


void* pipelineStage(void* ptr) {
	pipelineArgs* args = (pipelineArgs*)ptr;
	queue<int>& input = *args->inputptr;
	parallelPatterns pattern;
	for (int i = 0; i < args->length; i++) {
		int data = pattern.ReadFromQueue(args->inputptr , args->inputLock, args->inputCond);
		int result = args->func(data);
		pattern.WriteToQueue(result, args->output, args->outputLock, args->outputCond);
	}
	return 0;
}

void parallelPatterns::WriteToQueue(int data, queue<int> &outputQueue, pthread_mutex_t &lock, pthread_cond_t &cond) {
	pthread_mutex_lock(&lock);
	outputQueue.push(data);
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&cond);
}


int parallelPatterns::ReadFromQueue(queue<int>* inputQueue, pthread_mutex_t &lock, pthread_cond_t &cond) {
	queue<int>& queue = *inputQueue;
	pthread_mutex_lock(&lock);
	if (queue.empty()) {
		pthread_cond_wait(&cond, &lock);
	}
	int data = queue.front();
	queue.pop();
	pthread_mutex_unlock(&lock);
	return data;
}