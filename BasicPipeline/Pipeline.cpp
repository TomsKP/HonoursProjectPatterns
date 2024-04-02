#include <iostream>
#include "projectPattern.h"
#include "tbb/tbb.h"
#include "tbb/flow_graph.h"

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
	for (int i = 0; i <= data; i++) {
		result = result + i;
		/*if (i == data) {
			cout << "result: " << result << endl;
		}*/
	}
	return data+1;
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
	/*queue<int> start;
	queue<int> end;
	for (int i = 0; i < 100000; i++) {
		start.push(i);
	}
	start.push(-1);

	pattern.pipelineMain(func, start, &end);*/

	
	
	//This implementation is taken and adapted from two sources on TBB pipeline
	//First one: https://stackoverflow.com/questions/61399069/tbb-parallel-pipeline-seems-to-run-in-order
	//Second one: https://oneapi-src.github.io/oneTBB/main/tbb_userguide/Working_on_the_Assembly_Line_pipeline.html
	//Second source is highly detailed, but I needed the first source to actually see an example of how it can be used
	int start = 0;

	tbb::parallel_pipeline(2,
		tbb::make_filter<void, int>(oneapi::tbb::filter_mode::serial_out_of_order, [&](tbb::flow_control& fc) -> int {
			if (start < 100000) {
				return start++;
			}
			fc.stop();
			return -1; // End of input
		}) &
		tbb::make_filter<int, int>(tbb::filter_mode::parallel, [](int data) -> int {
			// Apply function1
			return function4(data);
		}) &
		tbb::make_filter<int, int>(tbb::filter_mode::parallel, [](int data) -> int {
			// Apply function2
			return function4(data);
		}) &
		tbb::make_filter<int, int>(tbb::filter_mode::parallel, [](int data) -> int {
			// Apply function3
			return function4(data);
		}) &
		tbb::make_filter<int, void>(tbb::filter_mode::parallel, [](int data) {
			// Apply function4
			function4(data);
		})
		);


	return 0;
}