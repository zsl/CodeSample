#include <windows.h>
#include <process.h>
#include <iostream>


bool DeleteDirectory(const std::wstring& strFullPath)
{
	wchar_t sourceBuffer[MAX_PATH] = {0};

	//memcpy(sourceBuffer, Util::FormatPath(strFullPath, false).c_str(), strFullPath.size()*sizeof(wchar_t));
	memcpy(sourceBuffer, strFullPath.c_str(), strFullPath.size()*sizeof(wchar_t));

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp, sizeof(FileOp));

	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = sourceBuffer;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;

	return SHFileOperation(&FileOp) == 0;
}

int main(){
	using namespace std;
	wcout.imbue(locale("chs"));

	wchar_t *srcPath = L"E:\\workspace\\test\\bin\\bag.jar";
	wchar_t *dstPath = L"F:\\bag.jar";
	
	bool checked = CopyFile(srcPath, dstPath, false);
	if (checked){
		cout<<"�����ɹ�\n";

		wchar_t buf[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, buf);
		wcout<<buf<<endl;
		SetCurrentDirectory(L"f:\\");
		char *cmd = "\"jar xfv F:\\bag.jar\"";
		cout<<cmd;
		int rtcode = system(cmd);
		SetCurrentDirectory(buf);
		cout<<"��ѹ�ɹ�\n";
		SetFileAttributes(dstPath,GetFileAttributes(dstPath)&(~FILE_ATTRIBUTE_READONLY)); // ȥ��ֻ������
		checked = DeleteFile(dstPath);

		if (checked){
			cout<<"ɾ��jar�ɹ�\n";
		}
		else cout<<"ɾ��jarʧ��\n";

	}

	DeleteDirectory(L"F:\\b\\*");
	getchar();
}