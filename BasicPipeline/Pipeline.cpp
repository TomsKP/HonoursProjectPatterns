#include <iostream>
#include "projectPattern.h"

#include <queue>
#include <pthread.h>

using namespace std;

queue<int> stage1output;
queue<int> stage2output;

pthread_mutex_t mutex_stage1output_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_stage1output_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_stage2output_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_stage2output_cond = PTHREAD_COND_INITIALIZER;


parallelPatterns pattern;

int startArray[11] = { 1,2,3,4,5,6,7,8,9,10,-1 };
int endArray[11];


int function1(int data) {
	int result = data + 1;
	return result;
}
int function2(int data) {
	int result = data * 2;
	return result;
}
int function3(int data) {
	int result = data + 1;
	return result;
}
int function4(int data) {
	int result = 0;
	for (int i = 0; i < data; i++) {
		result = result + i;
	}
	return data;
}

void printArray(){
	for (int i = 0; i < 10; i++) {
		cout << endArray[i] << " ";
	}
	cout << endl;
}





int main() {
	//int (*func[])(int) = { function1, function2, function3 };
	int (*func[])(int) = { function4, function4, function4, function4 };
	queue<int> start;
	queue<int> end;
	for (int i = 0; i < 100000; i++) {
		start.push(i);
	}
	start.push(-1);
	pattern.pipelineMain(func, start, &end);
	

	return 0;
}