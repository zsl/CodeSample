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

   // ���ÿ�ݷ�ʽ����

   // ����Ŀ���ļ�
   ret = pShellLink->SetPath(_T("c:\\abc.txt"));

   // ���ù���Ŀ¼
   ret = pShellLink->SetWorkingDirectory(_T("c:\\"));

   // ���������в���
   ret = pShellLink->SetArguments(_T("����"));

   // ����������Ϣ
   ret = pShellLink->SetDescription(_T("���Կ�ݷ�ʽ"));

   // ����ͼ���ļ�
   ret = pShellLink->SetIconLocation(_T("e:\\test.ico"), 0);

   // �����ļ�

   IPersistFile *pPersistFile = NULL;
   ret = pShellLink->QueryInterface(IID_IPersistFile, (void **)&pPersistFile);
   if (FAILED(ret)){
       pShellLink->Release();
       std::cerr << "QueryInterface IPersistFile Failed.\n";
       return 1;
   }

   // ��ȡ����·��
   TCHAR strBuf[MAX_PATH];
   ret = SHGetSpecialFolderPath(NULL, strBuf, CSIDL_COMMON_DESKTOPDIRECTORY, TRUE);
   if (FAILED(ret)){
       std::cerr << "��ȡ����·��Failed.\n";
       pShellLink->Release();
       pPersistFile->Release();
       return 1;
   }

   _tcscat(strBuf, _T("\\test.lnk") );

   ret = pPersistFile->Save(strBuf, TRUE);

   // �ͷ�
   pPersistFile->Release();
   pShellLink->Release();

   std::cout << "Ok.\n";
}