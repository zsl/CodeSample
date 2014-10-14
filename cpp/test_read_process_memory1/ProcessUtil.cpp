#include "ProcessUtil.h"

#include <Windows.h>
#include <Psapi.h>  // for EnumProcess
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string>
#include <vector>

#include <boost/crc.hpp>
#include <memory>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "libeay32.lib")

//////////////////////////////////////////////////////////////////////////
// 根据base name获取进程句柄
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

					if (0 == stricmp(processName, szProcessName) ){
						return hProcess;
					}
				}
			} // end if Null != hProcess
		}// end if aProcess[i] != 0
	} // end for

	return NULL;
}



//////////////////////////////////////////////////////////////////////////
// 计算内存区间的摘要
void readMemory(HANDLE hProcess, void * const addrBase, void *pOut, const size_t size, SIZE_T *nSizeOfRead){
	BOOL ret = ReadProcessMemory(hProcess, addrBase, pOut, size, nSizeOfRead);
	if (!ret){
		throw CReadMemoryError(GetLastError());
	}
}


typedef void (* PmdAlgo)(const unsigned char *pBytes, const size_t size, unsigned char *pMd);

void CaculateMemoryMD(HANDLE hProcess, void * const addrBase, const size_t size, unsigned char *pMd, PmdAlgo mdAlgo){
	std::vector<unsigned char> vecMemData(size);

	unsigned char *pRawData = &vecMemData[0];
	SIZE_T nSizeOfRead = 0;
	readMemory(hProcess, addrBase, pRawData, size, &nSizeOfRead);

	mdAlgo(pRawData, nSizeOfRead, pMd);
}



void CaculateMemoryMd5(HANDLE hProcess, void * const addrBase, const size_t size, unsigned char *pMd5){
	std::vector<unsigned char> vecMemData(size);

	unsigned char *pRawData = &vecMemData[0];
	SIZE_T nSizeOfRead = 0;
	readMemory(hProcess, addrBase, pRawData, size, &nSizeOfRead);

	MD5(pRawData, nSizeOfRead, pMd5);
}



unsigned int CaculateMemoryCrc(HANDLE hProcess, void * const addrBase, const size_t size){
	std::vector<unsigned char> vecMemData(size);

	unsigned char * sap = &vecMemData[0];

	SIZE_T readedSize = 0;
	LONG ret = ReadProcessMemory(hProcess, addrBase, sap, size, &readedSize);

	boost::crc_32_type crc32;
	crc32.process_block(sap, sap + size);

	return crc32.checksum();
}

void CaculateMemorySha1(HANDLE hProcess, void * const addrBase, const size_t size, unsigned char *pSha1){
	std::vector<unsigned char> vecMemData(size);

	unsigned char *pRawData = &vecMemData[0];
	SIZE_T nSizeOfRead = 0;
	readMemory(hProcess, addrBase, pRawData, size, &nSizeOfRead);

	SHA1(pRawData, nSizeOfRead, pSha1);
}
