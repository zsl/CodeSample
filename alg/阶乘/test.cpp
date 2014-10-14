#include <iostream>

using namespace std;

int cal(int n)
{
    unsigned long j = 1;
    for (int i = 2; i <= n; ++i)
        j *= i;

    int result = 0;
    for (int i = 10; i < j; i *= 10){
        if (j % i ==0)
            result ++;
        else
            break;
    }
    return result;

}

int main()
{
    cout << cal(5) << endl;
    cout << cal(10) << endl;
}
