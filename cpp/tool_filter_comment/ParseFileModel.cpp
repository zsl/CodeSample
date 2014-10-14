#include "ParseFileModel.h"
ParseFileModel::ParseFileModel(void)
{
}


void ParseFileModel::FilterComment(string strFileFullName, vector<char>& vecChars)
{
	int nState=STATE_SCAN;
	FILE* fp = fopen(strFileFullName.c_str(),"r");
	char ch;
	if(fp == NULL)
		return;
	while(1)
	{
		ch=fgetc(fp);
		if(ch==EOF)
			break;
		if (ch == 0x20 || ch == 0x09)
		{
			continue;
		}

		switch(nState)
		{
		case STATE_IS_COMMENT_START:
			{				
				if(ch=='*')  //��������ע����ʼ
				{
					nState=STATE_IN_BLOCK_COMMENT;
				}
				else if(ch=='/')	//�ǵ���ע��
				{
					nState=STATE_IN_LINE_COMMENT;
				}
				else
				{	//�˳�
					nState=STATE_SCAN;
				}
				if (ch == '\n')
				{
					vecChars.push_back('\n');
				}
			}break;
		case STATE_IN_LINE_COMMENT:
			{
				if(ch=='\n')
				{
					//�˳���ע��״̬
					nState=STATE_SCAN;
					vecChars.push_back('\n');
				}
			}break;
		case STATE_IN_BLOCK_COMMENT:
			{
				if(ch == '*')		//�Ƿ��ǿ�ע�ͽ���
				{
					nState=STATE_IS_COMMENT_END;
				}
				if (ch == '\n')
				{
					vecChars.push_back('\n');
				}
			}break;

		case STATE_IS_COMMENT_END:
			{
				if(ch=='/')		//�������Ŀ�ע�ͽ���
				{
					nState=STATE_SCAN;
				}
				else if (ch == '*')
				{
					nState = STATE_IS_COMMENT_END;
				}
				else		//����ע��
				{
					nState=STATE_IN_BLOCK_COMMENT;
				}
				if (ch == '\n')
				{
					vecChars.push_back('\n');
				}

			}break;
		case STATE_SCAN:
			{
				if(ch=='/')	//�Ƿ���ע����ʼ
				{
					nState=STATE_IS_COMMENT_START;
				}
				else
				{
					//printf("%c",ch);
					vecChars.push_back(ch);
				}
			}break;
		}
	}
	fclose(fp);
}

void ParseFileModel::ReadLines(const vector<char>& vecChars, vector<LineNode>& vecLineNodes)
{

	static char buffer[0x1000];
	memset(buffer, 0x00, sizeof(buffer));
	int iPos = 0;

	
	int iIndex = 0;


	vector<char>::const_iterator it = vecChars.begin();
	for (; it != vecChars.end(); ++it)
	{
		LineNode lineNode;
		char ch = *it;
		if (ch == '\n')
		{
			string strLine = string(buffer);
			memset(buffer, 0x00, sizeof(buffer));

			lineNode.bBlank = "" == strLine ? true : false;
			lineNode.iLine = iIndex++;
			lineNode.strLine = strLine;

			vecLineNodes.push_back(lineNode);
			iPos = 0;
		}
		else
		{
			buffer[iPos++] = ch;
		}
	}
}




