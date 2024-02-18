#include <stdio.h>
#include <stdlib.h>
#include "projectPattern.h"

using namespace std;
parallelPatternFor pattern;
int args[8];

struct argData {
	int start;
	int end;
	int row;
	int threadCount;
};

int testArray[40];

void* additionFunction(void* ptr);

int main() {
	for (int i = 0; i < 40; i++) {
		testArray[i] = i;
	}

	for (int i = 0; i < 40; i++) {
		printf("%d \n", testArray[i]);
	}

	pattern.parallelFor(40, &additionFunction);

	for (int i = 0; i < 40; i++) {
		printf("%d \n", testArray[i]);
	}
}

void* additionFunction(void* ptr) {
	argData* args = (argData*) ptr;
	int start = args->start;
	int end = args->end;
	for (int i = start; i < end; i++) {
		testArray[i] = testArray[i] + 1;
	}
	return 0;
}