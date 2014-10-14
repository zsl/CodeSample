#ifndef __TIMECOUNTER__
#define __TIMECOUNTER__

#include <ctime>


/*
 一个计算程序运行时间的类，精确到秒
*/
class TimeCounter
{
public:
	TimeCounter(void);
	~TimeCounter(void);

	// 某个过程开始之前调用
	void processBegin(); 

	// 某个过程开始之后调用
	void processEnd();

	// 某个过程花费的时间
	double timeExpend();

private:
	//time_t begin;
	//time_t end;
	clock_t begin;
	clock_t end;

	int check;
};

#endif
