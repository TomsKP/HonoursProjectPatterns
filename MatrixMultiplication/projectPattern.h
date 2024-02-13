#pragma once

class parallelPatternFor {
public:
	
	void parallelFor(int, void* (*function)(void*), int *arg);
};