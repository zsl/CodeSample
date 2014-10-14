#include <windows.h>
#include <iostream>
#include <string>

void ListAllFileInDir(std::wstring &dir){
	using namespace std;
	wchar_t endChar = dir[dir.size()-1];
	wstring searchPath = dir + L"*";
	::WIN32_FIND_DATA wfd = {0};
	HANDLE hFind;

	hFind = FindFirstFile(searchPath.c_str(), &wfd);
	if (hFind != INVALID_HANDLE_VALUE){
		wcout<<wfd.cFileName<<endl;

		while (FindNextFile(hFind, &wfd)){
			//wprintf(L"%s\n", wfd.cFileName);
			wcout<<wfd.cFileName<<endl;
			memset(&wfd, 0, sizeof(wfd));
		}

		FindClose(hFind);
	}
}

int main(){
	wchar_t str[] = L"abcÖÐÎÄ\n";
	std::wcout.imbue(std::locale("chs"));
	std::wcout<<str;
	ListAllFileInDir(std::wstring(L"e:\\"));
	getchar();
}