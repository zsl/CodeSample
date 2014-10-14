#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;






enum{
	STATE_SCAN,					//ɨ��״̬
	STATE_IS_COMMENT_START,		//ע����ʼδ��
	STATE_IN_BLOCK_COMMENT,		//��ע��״̬
	STATE_IN_LINE_COMMENT,		//��ע��״̬
	STATE_IS_COMMENT_END,		//��ע�ͽ���δ��
};

struct LineNode 
{
	unsigned int iLine;
	bool bBlank;
	string strLine;
};


typedef vector<LineNode> VecLines;
typedef map<string, VecLines> FileMap;
typedef map<string, FileMap> PackageMap;

class ParseFileModel
{
private:
	ParseFileModel(void);
	PackageMap m_ParseInfo;
public:
	static ParseFileModel* GetInstance()
	{
		static ParseFileModel instance;
		return &instance;
	}


	void FilterComment(string strFileFullName, vector<char>& vecChars);
	void ReadLines(const vector<char>& vecChars, vector<LineNode>& vecLineNodes);
};	
