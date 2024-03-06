#pragma once

class parallelPatterns {
public:
	
	void parallelFor(int numItems, void* (*function)(void*));
	void parallelFor2D(int numRows, void* (*function)(void*));
	void pipelineInit(void* (*func[])(void*));
};