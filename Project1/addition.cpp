#include <stdio.h>
#include <stdlib.h>
#include "projectPattern.h"

using namespace std;
parallelPatternFor pattern;
int args[8];

int testArray[40];

void* additionFunction(void* ptr);

int main() {
	for (int i = 0; i < 40; i++) {
		testArray[i] = i;
	}

	for (int i = 0; i < 40; i++) {
		printf("%d \n", testArray[i]);
	}

	pattern.parallelFor(40, &additionFunction, args);

	for (int i = 0; i < 40; i++) {
		printf("%d \n", testArray[i]);
	}
}

void* additionFunction(void* ptr) {
	int threadID = (int) ptr;
	for (int i = args[threadID*2]; i < args[threadID*2+1]; i++) {
		testArray[i] = testArray[i] + 1;
	}
	return 0;
}