/**
 * @file consoleutil.cpp
 * @brief 控制台相关的函数
 * @author zsl
 * @version 1.0
 * @date 2012-10-25
 */

// Copyright (C) 2012 Feitian Technologies Co., Ltd. All rights reserved.

#include "consoleutil.h"

#include <iostream>
#include <string>
#include <conio.h>

#ifdef EXPORT_PYTHON__
#include <python.h>
#endif

using namespace std;

const int HK_UP       = EXT_FLAG | 0x48;
const int HK_DOWN     = EXT_FLAG | 0x50;
const int HK_LEFT     = EXT_FLAG | 0x4b;
const int HK_RIGHT    = EXT_FLAG | 0x4d;

const int HK_PAGEUP   = EXT_FLAG | 0x49;
const int HK_PAGEDOWN = EXT_FLAG | 0x51;

const int HK_INSERT   = EXT_FLAG | 0x52;
const int HK_DELETE   = EXT_FLAG | 0x53;

const int HK_HOME     = EXT_FLAG | 0x47;
const int HK_END      = EXT_FLAG | 0x4f;

typedef unsigned char uchar;
typedef unsigned int uint;


bool isUnicodeHead(const string &str, size_t pos)
{
    if (pos >= str.size() ) return false;

    for (size_t i = 0; i <= pos;){
        if ((uchar)str[i] > 0x7F){
            if (i == pos)
                return true;
            else i += 2;
        }
        else 
            i += 1;
    }
    return false;
}


const string &CmdReader::getline()const { return m_line; }
void CmdReader::setline(const string &line)
{

    if (!m_line.empty() )
        ClearInput(m_line.size(), m_cursorPos); // 暂时不考虑出错的情况，因为不知道这个函数啥时候出错，出错后是否可以恢复

    m_line = line;
    m_cursorPos = m_line.size();

    printf(line.c_str() );

}

void CmdReader::clearinput(){  setline(""); }

int CmdReader::getCursorPos()const{ return m_cursorPos; }
void CmdReader::setCursorPos(int pos)
{
    AdjustCursor(pos - m_cursorPos);
    m_cursorPos = pos; 
}

bool CmdReader::isRegistered(int key){ return m_registeredKey.count(key) > 0;}
void CmdReader::registerKey(int key){ m_registeredKey.insert(key); }
void CmdReader::unregisterKey(int key){m_registeredKey.erase(key);}
void CmdReader::unregisterAll(){  m_registeredKey.clear(); }

bool CmdReader::callback(int key){ return true;}

int CmdReader::readline()
{
    m_line.clear();
    // 离开时 恢复光标模式
    class CursorRecover{
    public:
        ~CursorRecover(){
            SetCursorInsert(false);
        }
    } recover;

    bool insertMode = false;
    bool firstUpArrow = false;

	m_cursorPos = 0;
    int key = 0;
	while(true) {
#if EXPORT_PYTHON__
        Py_BEGIN_ALLOW_THREADS
#endif
        key = _getch();
#if EXPORT_PYTHON__
        Py_END_ALLOW_THREADS
#endif
		if(GetAsyncKeyState(VK_CONTROL) < 0) {
            key |= CTRL_FLAG;
            if (0 == key || 0xE0 == key)
                key = _getch() | CTRL_FLAG;

            if(isRegistered(key) ) {
                if (callback(key) ){
                    //ClearInput(m_line.size(), m_cursorPos);
                    return key;
                }
            } // end if (isRegistered(key))
		} // end if VK_CONTROL
		else if(0x00 == key){//F1-F10 
			key = (_getch() | FN_FLAG);
			if(isRegistered(key)) {
				// ClearInput(m_line.size(), m_cursorPos);
                if (callback(key))
                    return key;
			}
		}
		else if(0xE0 == key){//EXT key 
			key = (_getch() | EXT_FLAG);
            if (isRegistered(key) ) {
                if(callback(key) ) {
                    // ClearInput(m_line.size(), m_cursorPos);
                    return key;
                }
            }
			switch(key) {
            case HK_LEFT:
				if(m_cursorPos > 0) {
                    int delta = -1;
                    if (m_cursorPos > 1 && isUnicodeHead(m_line, m_cursorPos - 2) )
                        delta -= 1;

					AdjustCursor(delta);
					m_cursorPos += delta;
				}
				break;
            case HK_RIGHT:
				if(m_cursorPos < m_line.size() ) {
                    int delta = 1;
                    if ( isUnicodeHead(m_line, m_cursorPos)  && m_cursorPos + 1 < m_line.size() )
                        delta += 1;

					AdjustCursor(delta);
					m_cursorPos += delta;
				}
				break;
            case HK_DELETE:
                if (m_cursorPos < m_line.size()){
                    int deleteLen = isUnicodeHead(m_line, m_cursorPos) ? 2 : 1;
                    for (int i = m_cursorPos + deleteLen; i < m_line.size(); ++i)
                        putchar(m_line[i]);

                    putchar(0);

                    AdjustCursor(m_cursorPos + deleteLen - m_line.size() -1 );
                    m_line.erase(m_cursorPos, deleteLen);
                }
                break;
            case HK_INSERT:
                insertMode = ! insertMode;
                SetCursorInsert(insertMode);
                break;
            case HK_UP:
                if ( m_history.empty() || m_historyIt == m_history.begin() ){
                    firstUpArrow = false;
                    break;
                }

                --m_historyIt; // m_historyIt 肯定不为：begin(), 已经检查过
                if (firstUpArrow){
                    firstUpArrow = false;
                }

                setline(*m_historyIt);
                break;
            case HK_DOWN:
                if ( m_history.empty() || m_historyIt == m_history.end() )
                    break;

                if (m_historyIt + 1 != m_history.end() ){
                    ++m_historyIt;
                    setline(*m_historyIt);
                }

                break;
            case HK_PAGEUP:
                if ( m_history.empty())
                    break;

                m_historyIt = m_history.begin();
                setline(*m_historyIt);
                break;
            case HK_PAGEDOWN:
                if ( m_history.empty())
                    break;

                m_historyIt = m_history.end() - 1;
                setline(*m_historyIt);
                break;
            case HK_HOME:
                setCursorPos(0);
                break;
            case HK_END:
                setCursorPos(m_line.size() );
                break;
            default:
                break;
            }// end switch
		}
		else if('\r' == key || '\n' == key){		//ENTER 
            putchar('\n');
			break;
		}
		else if('\b' == key) {	//BACKSPACE
			if(m_cursorPos > 0) { 
                int delta = -1;
                if (m_cursorPos > 1 && isUnicodeHead(m_line, m_cursorPos - 2) )
                    delta -= 1;

				AdjustCursor(delta);
                size_t linesize = m_line.size();
				for(uint i=m_cursorPos; i < linesize; i++) {
					putchar(m_line[i]);
					m_line[i-1] = m_line[i];
				}
                putchar(0);

				AdjustCursor(m_cursorPos - m_line.size()  - 1);

                m_line.erase(linesize + delta);
                
				m_cursorPos += delta;
			}
		}
		else if(0x1B == key) {//ESC
			ClearInput(m_line.size(), m_cursorPos);
			m_cursorPos = 0;
            m_line.clear();
		}
		else if(isprint(key) || key > 0x7F) {
            bool isUnicode = key > 0x7F;

			putchar(key);

            int keyleft = 0;
            if (isUnicode){
                keyleft = _getch();
                putchar(keyleft);
            }

			if(insertMode) {
                char newChar[] = {key, keyleft, 0};
                int replaceLen = isUnicodeHead(m_line, m_cursorPos) ? 2 : 1;
                m_line.replace(m_cursorPos, replaceLen, newChar);
                if (replaceLen != strlen(newChar)){
                    size_t linesize = m_line.size();
                    uint beginPos = m_cursorPos + (isUnicode ? 2 : 1);

                    for(uint i = beginPos; i < linesize; i++)
                        putchar(m_line[i]);

                    AdjustCursor(m_cursorPos - linesize + (isUnicode ? 2 : 1));
                }
			}// end if insertMode
			else {
                // 打印光标后的字符
                size_t linesize = m_line.size();
				for(uint i=m_cursorPos; i < linesize; i++)
					putchar(m_line[i]);
                // 将字符插入到当前光标所在的位置
                m_line.insert(m_cursorPos, 1, key);
                if (isUnicode)
                    m_line.insert(m_cursorPos + 1, 1, keyleft);

				AdjustCursor(m_cursorPos - linesize);
			}

			m_cursorPos++;
            if (isUnicode)
                m_cursorPos++;
		}
		else {//unknown key
			//ignored
		}
	}

    if ( !m_line.empty() ){
        if (m_history.empty() || m_line != m_history.back() ){
            m_history.push_back(m_line);
            m_historyIt = m_history.end();
        }
    }

	return 0;//non hot key
}

void DebugPrint(const char * format, ... ){
	char msg[512];
	va_list args;
	va_start (args, format);
	vsprintf (msg, format, args);
    va_end (args);
	
	OutputDebugStringA(msg);
}

bool AdjustCursor(int delta)
{
    CONSOLE_SCREEN_BUFFER_INFO screenBufInfo;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL ret = GetConsoleScreenBufferInfo(hStdOut,&screenBufInfo);
    if (!ret)
        return false;

    SHORT bufferX = screenBufInfo.dwSize.X;
    SHORT cursorX = screenBufInfo.dwCursorPosition.X;
        CHAR_INFO *lineBuf = new CHAR_INFO[bufferX];
        ReadConsoleLine(screenBufInfo.dwCursorPosition.Y, lineBuf, bufferX);
        delete []lineBuf;
    if (cursorX == 0 && delta < 0){
        screenBufInfo.dwCursorPosition.Y -= 1;
        screenBufInfo.dwCursorPosition.X = bufferX + delta;
        CHAR_INFO *lineBuf = new CHAR_INFO[bufferX];
        ReadConsoleLine(screenBufInfo.dwCursorPosition.Y, lineBuf, bufferX);
        delete []lineBuf;
    }
    else if (cursorX + delta >= bufferX){  // 如果需要换行
        screenBufInfo.dwCursorPosition.Y += 1;
        screenBufInfo.dwCursorPosition.X = 0;
    }
    else{
        screenBufInfo.dwCursorPosition.X += (SHORT)delta;
    }

    ret = SetConsoleCursorPosition(hStdOut, screenBufInfo.dwCursorPosition);
    if (!ret)
        return false;

    return true;
}


bool SetCursorInsert(bool bInsertMode/* = false*/)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.bVisible = TRUE;
	cursorInfo.dwSize = bInsertMode ? 100 : 20;
	return SetConsoleCursorInfo(hStdOut,&cursorInfo);
}

bool ClearInput(unsigned int maxlen, unsigned int cursorpos)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufInfo;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL ret = GetConsoleScreenBufferInfo(hStdOut,&screenBufInfo);
    if (!ret)
        return false;
	screenBufInfo.dwCursorPosition.X -= (SHORT)cursorpos;
	DWORD numberOfCharsWritten;
	ret = FillConsoleOutputCharacter(hStdOut,0,maxlen,screenBufInfo.dwCursorPosition,&numberOfCharsWritten);
    if (!ret)
        return false;
	return SetConsoleCursorPosition(hStdOut,screenBufInfo.dwCursorPosition);
}

bool ReadConsoleLine(int lineNu, CHAR_INFO *buffer, int bufLen)
{
    memset(buffer, 0, bufLen * sizeof CHAR_INFO);

    COORD bufferSize = {1, bufLen};
    COORD bufferCoord = {0, 0};
    SMALL_RECT readRegion = {0, lineNu, bufLen - 1, lineNu}; // left, top, right, bottom

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    BOOL ret = ReadConsoleOutputA(
            hStdout,
            buffer,
            bufferSize,
            bufferCoord,
            &readRegion
            );

    DebugPrint("ret(%d), read(%d), real(%d), last(%d), Y(%d)", 
            ret, bufLen, (int)readRegion.Right,(int)buffer[bufLen-1].Char.AsciiChar, lineNu);
    return ret;
}
