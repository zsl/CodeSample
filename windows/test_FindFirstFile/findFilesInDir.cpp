#include <Windows.h>

#include <vector>
#include <string>
#include <cstring>

#include <iostream>


template<typename Tchar>
bool listdir(const std::basic_string<Tchar> & path, std::vector<std::basic_string<Tchar> > & files, bool findHide = true);

template<typename Tchar>
bool listdir(const Tchar * path, std::vector<std::basic_string<Tchar> > & files, bool findHide = true)
{
    return listdir(std::basic_string<Tchar>(path), files, findHide);
}

template <>
bool listdir(const std::string &path, std::vector<std::string> &files, bool findHide)
{
	size_t index = path.rfind('\\');
	std::string spit;
	if(index != path.size() - 1)
		spit.append("\\");
	spit.append("*");

	std::string searchName = path + spit;
	WIN32_FIND_DATAA data;
	HANDLE handle = ::FindFirstFileA(searchName.c_str(), &data);
	if(handle != INVALID_HANDLE_VALUE)
	{
		do
		{
            // 如果想要判断是否是目录,data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			if(!findHide && (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
				continue;

            if (0 != strcmp(data.cFileName, ".") && 0 != strcmp(data.cFileName, ".."))
                files.push_back(data.cFileName);
            files.push_back(data.cFileName);

		}while(::FindNextFileA(handle, &data));

		::FindClose(handle);
	}
	else
	{
		return false;
	}

	return true;
}

template<>
bool listdir(const std::wstring &path, std::vector<std::wstring> &files, bool findHide)
{
	size_t index = path.rfind(L'\\');
	std::wstring spit;
	if(index != path.size() - 1)
		spit.append(L"\\");
	spit.append(L"*");

	std::wstring searchName = path + spit;
	WIN32_FIND_DATAW data;
	HANDLE handle = ::FindFirstFileW(searchName.c_str(), &data);
	if(handle != INVALID_HANDLE_VALUE)
	{
		do
		{
            // 如果想要判断是否是目录,data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			if(!findHide && (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
				continue;


            if (0 != wcscmp(data.cFileName, L".") && 0 != wcscmp(data.cFileName, L".."))
                files.push_back(data.cFileName);

		}while(::FindNextFileW(handle, &data));

		::FindClose(handle);
	}
	else
	{
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
    typedef std::basic_string<TCHAR> tstring;
    
    std::vector<tstring> files;
    bool ret = listdir(TEXT("e:\\zsl"), files);
    if (ret)
        for (size_t index = 0; index < files.size(); ++index) {
            std::wcout << files[index] << std::endl;
        }
    else
        std::cerr << "listdir error.\n";
}