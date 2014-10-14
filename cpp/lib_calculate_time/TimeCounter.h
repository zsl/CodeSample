#ifndef __TIMECOUNTER__
#define __TIMECOUNTER__

#include <ctime>


/*
 һ�������������ʱ����࣬��ȷ����
*/
class TimeCounter
{
public:
	TimeCounter(void);
	~TimeCounter(void);

	// ĳ�����̿�ʼ֮ǰ����
	void processBegin(); 

	// ĳ�����̿�ʼ֮�����
	void processEnd();

	// ĳ�����̻��ѵ�ʱ��
	double timeExpend();

private:
	//time_t begin;
	//time_t end;
	clock_t begin;
	clock_t end;

	int check;
};

#endif
