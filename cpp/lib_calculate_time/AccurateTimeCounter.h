#ifndef __ACCURATETIMECOUNTER__
#define __ACCURATETIMECOUNTER__

#include <Windows.h>

/*
 ¾«È·µ½ºÁÃë
*/
class AccurateTimeCounter
{
public:
	AccurateTimeCounter(void);
	~AccurateTimeCounter(void);

	void processBegin();
	void processEnd();
	double timeExpend();

private:
	LARGE_INTEGER begin;
	LARGE_INTEGER end;

	int check;
};

#endif