#include <Windows.h>
#include <Psapi.h>  // for EnumProcess
#include <string>
#include <iostream>
#include <fstream>
#include <boost/crc.hpp>
#include <openssl/sha.h>

#pragma comment(lib, "psapi.lib")

using namespace std;

HANDLE GetProcessHandleByBaseName(const char *processName){
	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return NULL;

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.
	CHAR szProcessName[MAX_PATH] = "<unknown>";
	for ( i = 0; i < cProcesses; i++ ){
		if( aProcesses[i] != 0 ){
			// Get a handle to the process.
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );

			// Get the process name.
			if (NULL != hProcess ){
				HMODULE hMod;
				DWORD cbNeeded;

				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ){
					GetModuleBaseNameA( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(CHAR) );

					if (0 == stricmp("inq70.exe", szProcessName) ){
						return hProcess;
					}
				}
			} // end if Null != hProcess
		}// end if aProcess[i] != 0
	} // end for

	return NULL;
}

HMODULE *GetProcessModules(HANDLE hProcess, size_t *sizeModule){
	HMODULE *hMods = new HMODULE[1];

	DWORD cbNeeded = 0;
	if( !EnumProcessModules(hProcess, hMods, sizeof(HMODULE), &cbNeeded)){
		delete[] hMods;
		return NULL;
	}
	if (cbNeeded > 1){
		delete[] hMods;
		hMods = new HMODULE[cbNeeded / sizeof HMODULE];
		if( !EnumProcessModules(hProcess, hMods, cbNeeded, &cbNeeded)){
			delete[] hMods;
			return NULL;
		}
	}
	*sizeModule = cbNeeded / sizeof HMODULE;

	return hMods;
}

void ReleaseModules(HMODULE *pmMods){
	delete[] pmMods;
}

HMODULE GetModuleByName(const char *name, HANDLE hProcess, HMODULE hMods[], size_t sizeMods){
	char szModName[MAX_PATH];
	for (size_t i = 0; i < sizeMods; i++ ){
		GetModuleBaseNameA( hProcess, hMods[i], szModName, MAX_PATH );
		if (0 == stricmp(name, szModName) ){
			return hMods[i];
		}
	}
	return NULL;	
}

unsigned short GetModuleMemoryCRC(HANDLE hProcess, HMODULE hMod){
	CHAR szModName[MAX_PATH];
	// Get the full path to the module's file.
	if ( GetModuleFileNameExA(hProcess, hMod, szModName, sizeof(szModName)/sizeof(TCHAR))){

		printf("%s\n", szModName);
		printf("True virtual addr : 0x%08X\n", hMod);

		// 打印模块的基地址，首选virtual addr, virtual size
		ifstream ifs(szModName, ios::binary);

		// dos header
		IMAGE_DOS_HEADER dosHeader;

		ifs.read((char *)&dosHeader, sizeof(IMAGE_DOS_HEADER) );

		// Image Header
		ifs.seekg(dosHeader.e_lfanew, ios::beg);
		ifs.seekg(sizeof(DWORD), ios::cur);  // 跳过4字节，PE文件标记"PE"
		IMAGE_FILE_HEADER fileHeader;
		ifs.read((char *)&fileHeader, sizeof(IMAGE_FILE_HEADER) );

		WORD numSections = fileHeader.NumberOfSections;
		WORD sizeOptionalHeader = fileHeader.SizeOfOptionalHeader;

		// Optional Header
		IMAGE_OPTIONAL_HEADER optionalHeader;
		ifs.read((char *)&optionalHeader, sizeOptionalHeader); 

		printf("Base of Code: 0x%08X\n", optionalHeader.BaseOfCode);
		printf("Size of Code: 0x%08X\n", optionalHeader.SizeOfCode);
		printf("Image Base  : 0x%08X\n", optionalHeader.ImageBase);

		// Section Table
		size_t numCodeSection = 0;

		DWORD addrCodeSection;
		DWORD sizeCodeSection;
		DWORD sizeRawData;

		IMAGE_SECTION_HEADER sectionHeader;
		for (int i = 0; i < numSections; ++i){
			ifs.read((char *)&sectionHeader, sizeof(sectionHeader) );
			if (sectionHeader.Characteristics & IMAGE_SCN_CNT_CODE){ // 代码段
				++numCodeSection;

				addrCodeSection = sectionHeader.VirtualAddress;
				sizeCodeSection = sectionHeader.Misc.VirtualSize;
				sizeRawData     = sectionHeader.SizeOfRawData;
				break;
			}
		}

		printf("CodeSection_VirtualAddress : 0x%08X\n", addrCodeSection);
		printf("CodeSection_VirtualSize    : 0x%08X\n", sizeCodeSection);
		printf("CodeSection_RawSize        : 0x%08X\n", sizeRawData);
		printf("\n");
		// 读取代码段
		// addr: ImageBase + BaseOfCode
		// size: SizeOfCode

		//ReadProcessMemory(hProcess, );

		ifs.close();
	}
	return 0;
}

#pragma comment(lib, "libeay32.lib")
#include <boost/crc.hpp>
int main( )
{
	unsigned char arr[] = {0, 1, 2, 0};
	unsigned char sha1[20];
	SHA1(arr, sizeof arr / sizeof arr[0], sha1);
	
	boost::crc_32_type crc32;
	crc32.process_block(arr, arr + 4);

	unsigned int sum =  crc32.checksum();

	char *processName = "inq70.exe";
	HANDLE hProcess = GetProcessHandleByBaseName(processName);

	// 从0x10000 - 0x10000 + 0x100
	const size_t addrBase = 0x10000;
	const size_t nSize = 0x100;
	char buf[nSize];
	SIZE_T nNumberRead = 0;
	ReadProcessMemory(hProcess, LPCVOID(addrBase), buf, nSize, &nNumberRead);

	cout << "NumberRead:" << nNumberRead << endl;

	ofstream ofs("f:\\memoryc.dat");
	ofs.write(buf, nNumberRead);
	ofs.close();

	size_t sizeMod = 0;
	HMODULE *hMods = GetProcessModules(hProcess, &sizeMod);

	if (NULL == hMods) return 1;

	for (size_t i = 0; i < sizeMod; i++ ){
		CHAR szModName[MAX_PATH];
		// Get the full path to the module's file.
		if ( GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName)/sizeof(TCHAR))){

			printf("%s\n", szModName);
			printf("True virtual addr : 0x%08X\n", hMods[i]);

			// 打印模块的基地址，首选virtual addr, virtual size
			ifstream ifs(szModName, ios::binary);

			// dos header
			IMAGE_DOS_HEADER dosHeader;

			ifs.read((char *)&dosHeader, sizeof(IMAGE_DOS_HEADER) );

			// Image Header
			ifs.seekg(dosHeader.e_lfanew, ios::beg);
			ifs.seekg(sizeof(DWORD), ios::cur);  // 跳过4字节，PE文件标记"PE"
			IMAGE_FILE_HEADER fileHeader;
			ifs.read((char *)&fileHeader, sizeof(IMAGE_FILE_HEADER) );

			WORD numSections = fileHeader.NumberOfSections;
			WORD sizeOptionalHeader = fileHeader.SizeOfOptionalHeader;

			// Optional Header
			IMAGE_OPTIONAL_HEADER optionalHeader;
			ifs.read((char *)&optionalHeader, sizeOptionalHeader); 

			printf("Base of Code: 0x%08X\n", optionalHeader.BaseOfCode);
			printf("Size of Code: 0x%08X\n", optionalHeader.SizeOfCode);
			printf("Image Base  : 0x%08X\n", optionalHeader.ImageBase);

			// Section Table
			size_t numCodeSection = 0;

			DWORD addrCodeSection;
			DWORD sizeCodeSection;
			DWORD sizeRawData;

			IMAGE_SECTION_HEADER sectionHeader;
			for (int i = 0; i < numSections; ++i){
				ifs.read((char *)&sectionHeader, sizeof(sectionHeader) );
				if (sectionHeader.Characteristics & IMAGE_SCN_CNT_CODE){ // 代码段
					++numCodeSection;

					addrCodeSection = sectionHeader.VirtualAddress;
					sizeCodeSection = sectionHeader.Misc.VirtualSize;
					sizeRawData     = sectionHeader.SizeOfRawData;
					break;
				}
			}

			printf("CodeSection_VirtualAddress : 0x%08X\n", addrCodeSection);
			printf("CodeSection_VirtualSize    : 0x%08X\n", sizeCodeSection);
			printf("CodeSection_RawSize        : 0x%08X\n", sizeRawData);
			printf("\n");
			// 读取代码段
			// addr: ImageBase + BaseOfCode
			// size: SizeOfCode

			//ReadProcessMemory(hProcess, );

			ifs.close();
		}
	}

	CloseHandle(hProcess);

	ReleaseModules(hMods);

	getchar();

	return 0;
}

