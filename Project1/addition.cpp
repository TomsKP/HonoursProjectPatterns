#include <stdio.h>
#include <stdlib.h>
#include "projectPattern.h"
#include "tbb/tbb.h"

using namespace std;
parallelPatterns pattern;
int args[8];
int testArray[40];

struct argData {
	int start;
	int end;
	int threadCount;
};


//Ideas for this implementation came from: https://stackoverflow.com/questions/10607215/simplest-tbb-example and https://homepages.math.uic.edu/~jan/mcs572f16/mcs572notes/lec11.html
//Still not 100% on the working behind it, but fairly confident I know how to use TBB at this extremely basic level
class additionTBB {
public:
	void operator()(const tbb::blocked_range<int>& r) const {
		for (int i = r.begin(); i < r.end(); i++) {
			testArray[i] = testArray[i] + 1;
		}
	}
};



void* additionFunction(void* ptr);

int main() {
	for (int i = 0; i < 40; i++) {
		testArray[i] = i;
	}

	for (int i = 0; i < 40; i++) {
		printf("%d \n", testArray[i]);
	}

	//pattern.parallelFor(40, &additionFunction);
	tbb::parallel_for(tbb::blocked_range<int>(0, 40, 10), additionTBB());

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