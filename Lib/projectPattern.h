#pragma once
#include <queue>
#include "pthread.h"

class parallelPatterns {
public:
	
	void parallelFor(int numItems, void* (*function)(void*));
	void parallelFor2D(int numRows, void* (*function)(void*));
	void pipelineInit(void* (*func[])(void*));
	void pipelineMain(int (*func[])(int), std::queue<int> &start, std::queue<int> *end);

	template <typename T>
	void WriteToQueue(T data, std::queue<T> &outputQueue, pthread_mutex_t &lock, pthread_cond_t &cond);

	template<typename T>
	int ReadFromQueue(std::queue<T>* inputQueue, pthread_mutex_t &lock, pthread_cond_t &cond);
};