#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;






enum{
	STATE_SCAN,					//É¨Ãè×´Ì¬
	STATE_IS_COMMENT_START,		//×¢ÊÍÆðÊ¼Î´¾ö
	STATE_IN_BLOCK_COMMENT,		//¿é×¢ÊÍ×´Ì¬
	STATE_IN_LINE_COMMENT,		//ÐÐ×¢ÊÍ×´Ì¬
	STATE_IS_COMMENT_END,		//¿é×¢ÊÍ½áÊøÎ´¾ö
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
