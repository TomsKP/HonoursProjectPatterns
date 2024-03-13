#include <iostream>
#include "projectPattern.h"

#include <queue>
#include <pthread.h>

using namespace std;

queue<int> stage1output;
queue<int> stage2output;




//struct pipelineArgs {
//	pthread_mutex_t lock;
//	pthread_cond_t cond;
//	queue<int> input;
//	queue<int> output;
//};

pthread_mutex_t mutex_stage1output_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_stage1output_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_stage2output_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_stage2output_cond = PTHREAD_COND_INITIALIZER;


parallelPatterns pattern;

int startArray[10] = { 1,2,3,4,5,6,7,8,9,10 };
int endArray[10];

void* function1(void* arg){
	//pipelineArgs* arguments = (pipelineArgs*) arg;
	for (int i = 0; i < 10; i++) {
		pattern.WriteToQueue(startArray[i]+1, stage1output, mutex_stage1output_lock, mutex_stage1output_cond);
	}
	return 0;
}

void* function2(void* arg) {
	for (int i = 0; i < 10; i++) {
		int data = pattern.ReadFromQueue(stage1output, mutex_stage1output_lock, mutex_stage1output_cond);
		pattern.WriteToQueue(data * 2, stage2output, mutex_stage2output_lock, mutex_stage2output_cond);
	}
	return 0;
}

void* function3(void* arg) {
	for (int i = 0; i < 10; i++) {
		int data = pattern.ReadFromQueue(stage2output, mutex_stage2output_lock, mutex_stage2output_cond);
		endArray[i] = data + 1;
	}
	return 0;
}

void printArray(){
	for (int i = 0; i < 10; i++) {
		cout << endArray[i] << " ";
	}
	cout << endl;
}





int main() {
	void* (*funcs[])(void*) = {function1, function2, function3};
	pattern.pipelineInit(funcs);
	printArray();
	
	return 0;
}