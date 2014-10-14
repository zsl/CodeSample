/**
 * @file consoleutil.h
 * @brief 控制台相关函数
 * @author zsl
 * @version 1.0
 * @date 2012-10-25
 */

// Copyright (C) 2012 Feitian Technologies Co., Ltd. All rights reserved.
#ifndef CONSOLE_UTIL__
#define CONSOLE_UTIL__

#include <windows.h>
#include <string>
#include <set>
#include <vector>

#if _MSC_VER > 1500
#include <memory>
using std::shared_ptr;
using std::make_shared;
#else
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using boost::make_shared_ptr;
#endif


using std::string;
using std::set;
using std::vector;

enum KEY_FLAG{
    FN_FLAG = 0x0100, // 功能键标志
    EXT_FLAG  = 0x0200, 
    CTRL_FLAG = 0x0400, 
    ALT_FLAG  = 0x0800,
    SHIFT_FLAG = 0x1000,
};


class CmdReader{
public:

    int readline();
    void setline(const string &line);
    const string &getline()const;
    
    void clearinput();

    int getCursorPos()const;
    void setCursorPos(int pos);

    void registerKey(int key);
    void unregisterKey(int key);
    void unregisterAll();
    bool isRegistered(int key);
private:
    bool callback(int key);
private:
    string m_line;
    int m_cursorPos;
    set<int> m_registeredKey;

    vector<string> m_history;
    vector<string>::iterator m_historyIt;
};


bool AdjustCursor(int delta);
bool SetCursorInsert(bool bInsertMode = false);
bool ClearInput(unsigned int maxlen, unsigned int cursorpos);
bool ReadConsoleLine(int lineNu, CHAR_INFO *buffer, int bufLen);

#endif
