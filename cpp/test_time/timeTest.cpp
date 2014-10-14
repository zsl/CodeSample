#include <ctime>

#include <cstring>
#include <iostream>

using namespace std;

int getDayOfYear(int year, int month, int dayOfMonth)
{
    tm t;
    memset(&t, 0, sizeof tm);

    t.tm_year = year - 1900;
    t.tm_mon = month - 1;  // (0, 11)
    t.tm_mday = dayOfMonth; // (1, 31)
    t.tm_sec = 1;

    time_t timeOfDay = mktime(&t);
    tm *pTmOfDay = localtime(&timeOfDay);
    // cout << pTmOfDay->tm_year + 1900 << endl;

    return pTmOfDay->tm_yday + 1;  // 从第一天开始算起
}

int main()
{
    char *day = "20120101";
    int year, mon, mday;
    //sscanf(day, "%4d%2d%2d", &year, &mon, &mday);
    string strday = day;
    year = strtol( strday.substr(0, 4).c_str(), NULL, 10 );
    mon = strtol( strday.substr(4, 2).c_str(), NULL, 10 );
    mday = strtol( strday.substr(6, 2).c_str(), NULL, 10 );
    cout << year << mon << mday << endl;
    cout << getDayOfYear(year, mon, mday) << endl;
}
