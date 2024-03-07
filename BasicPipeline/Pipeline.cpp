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
		pthread_mutex_lock(&mutex_stage1output_lock);
		stage1output.push(startArray[i] + 1);
		pthread_mutex_unlock(&mutex_stage1output_lock);
	}
	return 0;
}

void* function2(void* arg) {
	for (int i = 0; i < 10; i++) {
		pthread_mutex_lock(&mutex_stage1output_lock);
		if (stage1output.empty()) {
			pthread_cond_wait(&mutex_stage1output_cond, &mutex_stage1output_lock);
		}
		int data = stage1output.front();
		stage1output.pop();
		pthread_mutex_unlock(&mutex_stage1output_lock);
		pthread_mutex_lock(&mutex_stage2output_lock);
		stage2output.push(data * 2);
		pthread_mutex_unlock(&mutex_stage2output_lock);
		pthread_cond_signal(&mutex_stage2output_cond);
	}
	return 0;
}

void* function3(void* arg) {
	for (int i = 0; i < 10; i++) {
		pthread_mutex_lock(&mutex_stage2output_lock);
		if (stage2output.empty()) {
			pthread_cond_wait(&mutex_stage2output_cond, &mutex_stage2output_lock);
		}
		int data = stage2output.front();
		stage2output.pop();
		pthread_mutex_unlock(&mutex_stage2output_lock);
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
	/*printArray();
	function1();
	printArray();
	function2();
	printArray();*/
	
	return 0;
}