#include "AccurateTimeCounter.h"
#include <cassert>

AccurateTimeCounter::AccurateTimeCounter(void):check()
{
}

AccurateTimeCounter::~AccurateTimeCounter(void)
{
}

void AccurateTimeCounter::processBegin(){
	assert(check == 0);
	check += 1;
	QueryPerformanceCounter(&begin);
}

void AccurateTimeCounter::processEnd(){
	assert(check == 1);
	QueryPerformanceCounter(&end);
	check += 1;
}

double AccurateTimeCounter::timeExpend(){
	assert(check == 2);
	check = 0;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return 1000 * 1.0 * (end.QuadPart - begin.QuadPart) / frequency.QuadPart;
}