/*
��֪��ĸ˳��d,g,c,f,b,e,a�� ��������һ���ַ���input��3��={��dca��,"dfa","cgd"} ������ĸ˳��������򲢴�ӡ
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
            return false;  // ���right�ĳ��� С�� left�ĳ��ȣ���ôleft > right
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
    // ��ʼ��vector
    vector<string> vec(argv + 1, argv + argc);
    
    // ��������
    mysort(vec);

    // ���
    for (int i = 0; i < vec.size(); ++i)
        cout << vec[i] << " ";

    cout << endl;
}
