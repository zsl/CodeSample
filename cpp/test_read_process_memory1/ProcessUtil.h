#ifndef PROCESS_UTIL_H__
#define PROCESS_UTIL_H__

#include <string>
#include <sstream>
#include <Windows.h>

std::string formatMessage(DWORD dwErrCode);

class CReadMemoryError : std::exception{
public:
	CReadMemoryError(DWORD dwErrCode):m_dwErrCode(m_dwErrCode){
		using namespace std;

		ostringstream oss("ReadMemoryError:");
		oss << dwErrCode << ", " << formatMessage(dwErrCode) << endl;
		m_strMsg.swap(oss.str());
	}
	const char* what() const throw(){
		return m_strMsg.c_str();
	}
private:
	DWORD m_dwErrCode;
	std::string m_strMsg;
};

//////////////////////////////////////////////////////////////////////////
// 计算内存区间的摘要
void readMemory(HANDLE hProcess, void * const addrBase, void *pOut, const size_t size, SIZE_T *nSizeOfRead);

HANDLE GetProcessHandleByBaseName(const char *processName);
void CaculateMemorySha1(HANDLE hProcess, void * const addrBase, const size_t size, unsigned char *pSha1);

#endif