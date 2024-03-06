#include <iostream>
#include "projectPattern.h"

using namespace std;

int startArray[10] = { 1,2,3,4,5,6,7,8,9,10 };

void* function1(){
	for (int i = 0; i < 10; i++) {
		startArray[i] = startArray[i] + 1;
	}
	return 0;
}

void* function2() {
	for (int i = 0; i < 10; i++) {
		startArray[i] = startArray[i] * 2;
	}
	return 0;
}

void printArray(){
	for (int i = 0; i < 10; i++) {
		cout << startArray[i] << " ";
	}
	cout << endl;
}

int main() {
	
	/*printArray();
	function1();
	printArray();
	function2();
	printArray();*/
	
	return 0;
}