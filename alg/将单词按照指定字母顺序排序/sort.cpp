/*
已知字母顺序【d,g,c,f,b,e,a】 请对输入的一组字符串input【3】={“dca”,"dfa","cgd"} 按照字母顺序进行排序并打印
 */

#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

map<char, int> alphaTable;

void initAlphaTable()
{
    alphaTable['d'] = 0;
    alphaTable['g'] = 1;
    alphaTable['c'] = 2;
    alphaTable['f'] = 3;
    alphaTable['b'] = 4;
    alphaTable['e'] = 5;
    alphaTable['a'] = 6;
}

bool myless(const string& left, const string& right)
{
    const size_t lsize = left.size();
    const size_t rsize = right.size();

    for (size_t i = 0; i < lsize; ++i){
        if (i >= rsize)
            return false;  // 如果right的长度 小于 left的长度，那么left > right
        if (left[i] == right[i])
            continue;
        else if (alphaTable[ left[i] ]  < alphaTable[ right[i] ])
            return true;
        else return false;
    }

    return true;
}

void mysort(vector<string> &vec)
{
    sort(vec.begin(), vec.end(), myless);
}

int main(int argc, char *argv[])
{
    if (argc < 2){
        cout << "please input the strings\n";
        exit(1);
    }

    initAlphaTable();
    // 初始化vector
    vector<string> vec(argv + 1, argv + argc);
    
    // 进行排序
    mysort(vec);

    // 输出
    for (int i = 0; i < vec.size(); ++i)
        cout << vec[i] << " ";

    cout << endl;
}
