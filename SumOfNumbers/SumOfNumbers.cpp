#include <stdio.h>
#include <stdlib.h>
#include "projectPattern.h"
#include "tbb/tbb.h"
#include <chrono>
#include <iostream>
using namespace std::chrono;

using namespace std;
parallelPatterns pattern;

int numberArray[350000];

struct argData {
	int start;
	int end;
	int threadCount;
};

void* sumFunction(void* ptr) {
	argData* args = (argData*)ptr;
	int start = args->start;
	int end = args->end;
	for (int i = start; i < end; i++) {
		int result = 0;
		for (int j = 0; j < numberArray[i]; j++) {
			result = result + j;
		}
	}
	return 0;
}

int main() {
	for (int i = 0; i < 350000; i++) {
		numberArray[i] = i;
	}

	auto start = high_resolution_clock::now();
	pattern.parallelFor(350000, sumFunction);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << duration.count() << std::endl;
}