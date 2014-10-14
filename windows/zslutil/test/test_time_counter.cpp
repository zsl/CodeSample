#include "TimeCounter.h"

#include <iostream>

int main()
{
    CTimeCounter tc;

    tc.start();
    Sleep(1000);
    tc.stop();

    std::cout << tc.elapsed() << std::endl;

    tc.resume();
    Sleep(1000);
    std::cout << tc.elapsed() << std::endl;
}