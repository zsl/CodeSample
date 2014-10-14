#include <iostream>

#include <Windows.h>
#include <tchar.h> // for _tcscat

#include <ObjBase.h> // for CoInitialize

#include <ShlObj.h> // for SHGetSpecialFolderPath
#include <ShObjIdl.h> // for IShellLink
#include <ShlGuid.h>  // for IID_IShellLink

#pragma comment(lib, "shell32.lib") // for SHGetSpecialFolderPath

#define _T TEXT

class ComIniter{
public:
    ComIniter(){
        CoInitialize(NULL);
    }

    ~ComIniter(){
        CoUninitialize();
    }
} g_comIniter;

int main(int argc, char *argv[]) {

   HRESULT ret;

   IShellLink *pShellLink = NULL;
   ret = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pShellLink);

   if (FAILED(ret)){
       std::cerr << "CoCreateInstance Failed" << std::endl;
       return 1;
   }

   // 设置快捷方式属性

   // 设置目标文件
   ret = pShellLink->SetPath(_T("c:\\abc.txt"));

   // 设置工作目录
   ret = pShellLink->SetWorkingDirectory(_T("c:\\"));

   // 设置命令行参数
   ret = pShellLink->SetArguments(_T("参数"));

   // 设置描述信息
   ret = pShellLink->SetDescription(_T("测试快捷方式"));

   // 设置图标文件
   ret = pShellLink->SetIconLocation(_T("e:\\test.ico"), 0);

   // 保存文件

   IPersistFile *pPersistFile = NULL;
   ret = pShellLink->QueryInterface(IID_IPersistFile, (void **)&pPersistFile);
   if (FAILED(ret)){
       pShellLink->Release();
       std::cerr << "QueryInterface IPersistFile Failed.\n";
       return 1;
   }

   // 获取桌面路径
   TCHAR strBuf[MAX_PATH];
   ret = SHGetSpecialFolderPath(NULL, strBuf, CSIDL_COMMON_DESKTOPDIRECTORY, TRUE);
   if (FAILED(ret)){
       std::cerr << "获取桌面路径Failed.\n";
       pShellLink->Release();
       pPersistFile->Release();
       return 1;
   }

   _tcscat(strBuf, _T("\\test.lnk") );

   ret = pPersistFile->Save(strBuf, TRUE);

   // 释放
   pPersistFile->Release();
   pShellLink->Release();

   std::cout << "Ok.\n";
}