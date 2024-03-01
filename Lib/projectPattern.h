#pragma once

class parallelPatternFor {
public:
	
	void parallelFor(int numItems, void* (*function)(void*));
	void parallelFor2D(int numRows, void* (*function)(void*));
};