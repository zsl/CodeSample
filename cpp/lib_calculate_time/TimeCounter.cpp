#include "TimeCounter.h"
#include <cassert>

TimeCounter::TimeCounter(void):check()
{
}

TimeCounter::~TimeCounter(void)
{
}

void TimeCounter::processBegin(){
	assert(check == 0);
	check += 1;
	//begin = time(NULL);
	begin = clock();
}

void TimeCounter::processEnd(){
	assert(check == 1);
	//end = time(NULL);
	end = clock();
	check += 1;
}

double TimeCounter::timeExpend(){
	assert(check == 2);
	check = 0;
	//return difftime(end, begin);
	return 1.0 * (end - begin) / CLOCKS_PER_SEC;
}
