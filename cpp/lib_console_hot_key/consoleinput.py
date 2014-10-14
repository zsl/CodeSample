#coding:gbk
# Copyright (C) 2012 Feitian Technologies Co., Ltd. All rights reserved.
##
# @file consoleinput.py
# @brief 读取控制台输入，可以读取快捷键
# @author zsl
# @version 1.0
# @date 2012-10-24



VK_BACK    = 0x08
VK_TAB     = 0x09
VK_CONTROL = 0x11
VK_MENU    = 0x12 # ALT


from consoleutil import *

FN_FLAG   = KEY_FLAG.FN_FLAG  # 功能键标志
EXT_FLAG  = KEY_FLAG.EXT_FLAG

CTRL_FLAG = KEY_FLAG.CTRL_FLAG
ALT_FLAG  = KEY_FLAG.ALT_FLAG
#EXT_FLAG = KEY_FLAG.EXT_FLAG

# 注：如果要添加一个快捷键，需要定义一个常量，并且在KEY_LIST中添加这个常量

# Ctrl + a ~ z
HK_CTRL_A = CTRL_FLAG | 0x01
HK_CTRL_B = CTRL_FLAG | 0x02
HK_CTRL_C = CTRL_FLAG | 0x03
HK_CTRL_D = CTRL_FLAG | 0x04
HK_CTRL_E = CTRL_FLAG | 0x05
HK_CTRL_F = CTRL_FLAG | 0x06
HK_CTRL_G = CTRL_FLAG | 0x07
HK_CTRL_H = CTRL_FLAG | 0x08
HK_CTRL_I = CTRL_FLAG | 0x09
HK_CTRL_J = CTRL_FLAG | 0x0a
HK_CTRL_K = CTRL_FLAG | 0x0b
HK_CTRL_L = CTRL_FLAG | 0x0c
HK_CTRL_M = CTRL_FLAG | 0x0d
HK_CTRL_N = CTRL_FLAG | 0x0e
HK_CTRL_O = CTRL_FLAG | 0x0f
HK_CTRL_P = CTRL_FLAG | 0x10
HK_CTRL_Q = CTRL_FLAG | 0x11
HK_CTRL_R = CTRL_FLAG | 0x12
HK_CTRL_S = CTRL_FLAG | 0x13
HK_CTRL_T = CTRL_FLAG | 0x14
HK_CTRL_U = CTRL_FLAG | 0x15
HK_CTRL_V = CTRL_FLAG | 0x16
HK_CTRL_W = CTRL_FLAG | 0x17
HK_CTRL_X = CTRL_FLAG | 0x18
HK_CTRL_Y = CTRL_FLAG | 0x19
HK_CTRL_Z = CTRL_FLAG | 0x1a

# F1 ~ F10 第一次读取一个0
HK_F1	=	FN_FLAG | 0x3b
HK_F2	=	FN_FLAG | 0x3c
HK_F3	=	FN_FLAG | 0x3d
HK_F4	=	FN_FLAG | 0x3e
HK_F5	=	FN_FLAG | 0x3f
HK_F6	=	FN_FLAG | 0x40
HK_F7	=	FN_FLAG | 0x41
HK_F8	=	FN_FLAG | 0x42
HK_F9	=	FN_FLAG | 0x43
HK_F10	=	FN_FLAG | 0x44
# F11 ~ F12 第一次读取0xE0
HK_F11	=	EXT_FLAG | 0x85
HK_F12	=	EXT_FLAG | 0x86

# Ctrl + Fn
HK_CTRL_F1	=	CTRL_FLAG | 0x5e
HK_CTRL_F2	=	CTRL_FLAG | 0x5f
HK_CTRL_F3	=	CTRL_FLAG | 0x60
HK_CTRL_F4	=	CTRL_FLAG | 0x61
HK_CTRL_F5	=	CTRL_FLAG | 0x62
HK_CTRL_F6	=	CTRL_FLAG | 0x63
HK_CTRL_F7	=	CTRL_FLAG | 0x64
HK_CTRL_F8	=	CTRL_FLAG | 0x65
HK_CTRL_F9	=	CTRL_FLAG | 0x66
HK_CTRL_F10	=	CTRL_FLAG | 0x67
HK_CTRL_F11	=	CTRL_FLAG | 0x89
HK_CTRL_F12	=	CTRL_FLAG | 0x8A

# Shift + Fn
HK_SHIFT_F1  = EXT_FLAG | 0x54
HK_SHIFT_F2  = EXT_FLAG | 0x55
HK_SHIFT_F3  = EXT_FLAG | 0x56
HK_SHIFT_F4  = EXT_FLAG | 0x57
HK_SHIFT_F5  = EXT_FLAG | 0x58
HK_SHIFT_F6  = EXT_FLAG | 0x59
HK_SHIFT_F7  = EXT_FLAG | 0x5A
HK_SHIFT_F8  = EXT_FLAG | 0x5B
HK_SHIFT_F9  = EXT_FLAG | 0x5C
HK_SHIFT_F10 = EXT_FLAG | 0x5D
HK_SHIFT_F11 = EXT_FLAG | 0x87
HK_SHIFT_F12 = EXT_FLAG | 0x88

# EXT
HK_UP       = EXT_FLAG | 0x48
HK_DOWN     = EXT_FLAG | 0x50
HK_LEFT     = EXT_FLAG | 0x4b
HK_RIGHT    = EXT_FLAG | 0x4d

HK_PAGEUP   = EXT_FLAG | 0x49 
HK_PAGEDOWN = EXT_FLAG | 0x51

HK_HOME     = EXT_FLAG | 0x47
HK_END      = EXT_FLAG | 0x4f

HK_INSERT   = EXT_FLAG | 0x52
HK_DELETE   = EXT_FLAG | 0x53

# Ctrl + EXT
HK_CTRL_UP       = EXT_FLAG | 0x8d
HK_CTRL_DWON     = EXT_FLAG | 0x91
HK_CTRL_LEFT     = EXT_FLAG | 0x73
HK_CTRL_RIGHT    = EXT_FLAG | 0x74

HK_CTRL_PAGEUP   = EXT_FLAG | 0x86
HK_CTRL_PAGEDOWN = EXT_FLAG | 0x76

HK_CTRL_HOME     = EXT_FLAG | 0x77
HK_CTRL_END      = EXT_FLAG | 0x75

HK_CTRL_INSERT   = EXT_FLAG | 0x92
HK_CTRL_DELETE   = EXT_FLAG | 0x93

KEY_LIST = [
    # Ctrl + a ~ z
    ('HK_CTRL_A' , HK_CTRL_A),
    ('HK_CTRL_B' , HK_CTRL_B),
    ('HK_CTRL_C' , HK_CTRL_C),
    ('HK_CTRL_D' , HK_CTRL_D),
    ('HK_CTRL_E' , HK_CTRL_E),
    ('HK_CTRL_F' , HK_CTRL_F),
    ('HK_CTRL_G' , HK_CTRL_G),
    ('HK_CTRL_H' , HK_CTRL_H),
    ('HK_CTRL_I' , HK_CTRL_I),
    ('HK_CTRL_J' , HK_CTRL_J),
    ('HK_CTRL_K' , HK_CTRL_K),
    ('HK_CTRL_L' , HK_CTRL_L),
    ('HK_CTRL_M' , HK_CTRL_M),
    ('HK_CTRL_N' , HK_CTRL_N),
    ('HK_CTRL_O' , HK_CTRL_O),
    ('HK_CTRL_P' , HK_CTRL_P),
    ('HK_CTRL_Q' , HK_CTRL_Q),
    ('HK_CTRL_R' , HK_CTRL_R),
    ('HK_CTRL_S' , HK_CTRL_S),
    ('HK_CTRL_T' , HK_CTRL_T),
    ('HK_CTRL_U' , HK_CTRL_U),
    ('HK_CTRL_V' , HK_CTRL_V),
    ('HK_CTRL_W' , HK_CTRL_W),
    ('HK_CTRL_X' , HK_CTRL_X),
    ('HK_CTRL_Y' , HK_CTRL_Y),
    ('HK_CTRL_Z' , HK_CTRL_Z),

    # F1 ~ F10 第一次读取一个0
    ('HK_F1' , HK_F1),
    ('HK_F2' , HK_F2),
    ('HK_F3' , HK_F3),
    ('HK_F4' , HK_F4),
    ('HK_F5' , HK_F5),
    ('HK_F6' , HK_F6),
    ('HK_F7' , HK_F7),
    ('HK_F8' , HK_F8),
    ('HK_F9' , HK_F9),
    ('HK_F10' , HK_F10),
    # F11 ~ F12 第一次读取0xE0
    ('HK_F11' , HK_F11),
    ('HK_F12' , HK_F12),

    # Ctrl + Fn
    ('HK_CTRL_F1' , HK_CTRL_F1),
    ('HK_CTRL_F2' , HK_CTRL_F2),
    ('HK_CTRL_F3' , HK_CTRL_F3),
    ('HK_CTRL_F4' , HK_CTRL_F4),
    ('HK_CTRL_F5' , HK_CTRL_F5),
    ('HK_CTRL_F6' , HK_CTRL_F6),
    ('HK_CTRL_F7' , HK_CTRL_F7),
    ('HK_CTRL_F8' , HK_CTRL_F8),
    ('HK_CTRL_F9' , HK_CTRL_F9),
    ('HK_CTRL_F10' , HK_CTRL_F10),
    ('HK_CTRL_F11' , HK_CTRL_F11),
    ('HK_CTRL_F12' , HK_CTRL_F12),

    # Shift + Fn
    ('HK_SHIFT_F1' , HK_SHIFT_F1),
    ('HK_SHIFT_F2' , HK_SHIFT_F2),
    ('HK_SHIFT_F3' , HK_SHIFT_F3),
    ('HK_SHIFT_F4' , HK_SHIFT_F4),
    ('HK_SHIFT_F5' , HK_SHIFT_F5),
    ('HK_SHIFT_F6' , HK_SHIFT_F6),
    ('HK_SHIFT_F7' , HK_SHIFT_F7),
    ('HK_SHIFT_F8' , HK_SHIFT_F8),
    ('HK_SHIFT_F9' , HK_SHIFT_F9),
    ('HK_SHIFT_F10' , HK_SHIFT_F10),
    ('HK_SHIFT_F11' , HK_SHIFT_F11),
    ('HK_SHIFT_F12' , HK_SHIFT_F12),
    # EXT
    ('HK_UP' , HK_UP),
    ('HK_DWON' , HK_DOWN),
    ('HK_LEFT' , HK_LEFT),
    ('HK_RIGHT' , HK_RIGHT),

    ('HK_PAGEUP' , HK_PAGEUP), 
    ('HK_PAGEDOWN' , HK_PAGEDOWN),

    ('HK_HOME' , HK_HOME),
    ('HK_END' , HK_END),

    ('HK_INSERT' , HK_INSERT),
    ('HK_DELETE' , HK_DELETE),

    # Ctrl + EXT
    ('HK_CTRL_UP' , HK_CTRL_UP),
    ('HK_CTRL_DWON' , HK_CTRL_DWON),
    ('HK_CTRL_LEFT' , HK_CTRL_LEFT),
    ('HK_CTRL_RIGHT' , HK_CTRL_RIGHT),

    ('HK_CTRL_PAGEUP' , HK_CTRL_PAGEUP),
    ('HK_CTRL_PAGEDOWN' , HK_CTRL_PAGEDOWN),

    ('HK_CTRL_HOME' , HK_CTRL_HOME),
    ('HK_CTRL_END' , HK_CTRL_END),

    ('HK_CTRL_INSERT' , HK_CTRL_INSERT),
    ('HK_CTRL_DELETE' , HK_CTRL_DELETE),
]

KEY_MAP = dict( KEY_LIST + [ (v[1], v[0]) for v in KEY_LIST] )

import ctypes
# SHORT GetAsyncKeyState(int vKey);
GetAsyncKeyState = ctypes.windll.user32.GetAsyncKeyState

# int _getch();
getch = ctypes.cdll.msvcrt._getch #读取箭头第一个字符是0xE0, 读取Fn第一个字符是0

# int putchar(int c);
putchar = ctypes.cdll.msvcrt.putchar

if __name__ == '__main__':
    print 'input "quit" to quit'
    cmdreader = CmdReader()
    while True:
        print '>',
        line = cmdreader.readline()
        if line == 'quit':
            break
        print line
