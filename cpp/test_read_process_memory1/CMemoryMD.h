#ifndef CMEMORY_MD__
#define CMEMORY_MD__

#include "m_util.h"
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

class CMemoryMD{
public:
	virtual ~CMemoryMD(){ delete[] pMd; }

public:
	// 计算MD
	void caculateMD(HANDLE hProcess, void * const addrBase, const size_t size){
		std::vector<unsigned char> vecMemData(size);

		unsigned char *pRawData = &vecMemData[0];
		SIZE_T nSizeOfRead = 0;
		readMemory(hProcess, addrBase, pRawData, size, &nSizeOfRead);

		mdAlgo(pRawData, nSizeOfRead);
	}
	// 获取摘要长度
	virtual size_t getLen()const = 0;

	unsigned char *getMD(){ return pMd; }
	
	
protected:
	// 摘要算法
	virtual void mdAlgo(const unsigned char *pBytes, const size_t size) = 0;

protected:
	unsigned char *pMd;
};

class CMemoryCrc32 : public CMemoryMD{
public:
	CMemoryCrc32(){
		pMd = new unsigned char[CRC32_COUNT];
		memset(pMd, 0, CRC32_COUNT);
	}


	size_t getLen()const{
		return CRC32_COUNT;
	}
	
protected:
	void mdAlgo(const unsigned char *pBytes, const size_t size){
		CRC32(pBytes, size, pMd);
	}

private:
	static const size_t CRC32_COUNT = 4;
};

class CMemorySha1 : public CMemoryMD{
public:
	CMemorySha1(){
		pMd = new unsigned char[SHA1_COUNT];
		memset(pMd, 0, SHA1_COUNT);
	}

	size_t getLen()const {
		return SHA1_COUNT;
	}

protected:
	void mdAlgo(const unsigned char *pBytes, const size_t size){
		SHA1(pBytes, size, pMd);
	}

private:
	static const size_t SHA1_COUNT = 20;
};

#endif