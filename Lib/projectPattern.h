#pragma once
#include <queue>
#include "pthread.h"

class parallelPatterns {
public:
	
	void parallelFor(int numItems, void* (*function)(void*));
	void parallelFor2D(int numRows, void* (*function)(void*));
	void pipelineInit(void* (*func[])(void*));
	void pipelineMain(int (*func[])(int), std::queue<int> &start, std::queue<int> &end, int length);

	void WriteToQueue(int data, std::queue<int> &outputQueue, pthread_mutex_t &lock, pthread_cond_t &cond);
	int ReadFromQueue(std::queue<int>* inputQueue, pthread_mutex_t &lock, pthread_cond_t &cond);
};