#include <Windows.h>

#include <boost/crc.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>

std::string formatMessage(DWORD dwErrCode){

	DWORD systemLocal = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	HLOCAL hlocal = NULL;
	BOOL bRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		NULL, dwErrCode, systemLocal, (char *)&hlocal, 0, NULL);

	if (!bRet || hlocal == NULL)
		return std::string("cannot find msg.");

	std::string msg = (char *)LocalLock(hlocal);
	LocalFree(hlocal);

	return msg;
}

//////////////////////////////////////////////////////////////////////////
// 将字节数组转化为 uint32
unsigned int byteArrtoUint4(const char *arr, const size_t size = 4){
	assert(size > 0 && size <= 4);

	const unsigned char *arrData = (unsigned char *)arr;

	unsigned int result = 0;
	for (size_t i = 0; i < size; ++i){
		result <<= 8;
		result |= arrData[i];
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
// 将字节数组转换为字符串
std::string byteArrToHexString(char *arr, int len, char *spliter/* = ""*/){
	assert(len > 0);

	using namespace std;
	unsigned char *arrbyte = (unsigned char *)(arr);
#ifdef C_API__
	const size_t nLenOfSpliter = strlen(spliter);
	vector<char> vecBuf;
	vecBuf.reserve(2 * len + nLenOfSpliter * (len - 1) + 1);
	char *buf = &vecBuf[0];
	size_t index = 0;

	for (size_t i = 0; i < len - 1; ++i){
		sprintf(buf + index, "%02X", (int)arrbyte[i]);
		index += 2;
		strcat(buf, spliter);
		index += nLenOfSpliter;
	}

	sprintf(buf + index, "%02X", (int)arrbyte[len - 1]);

	return std::string(buf);
#else
	ostringstream oss;
	oss << hex << uppercase << setfill('0');

	for (size_t i = 0; i < len - 1; ++i){
		oss << setw(2) << (int)arrbyte[i] << spliter;
	}

	oss << setw(2) << (int)arrbyte[len - 1];

	return oss.str();
#endif
}



//////////////////////////////////////////////////////////////////////////
// 摘要算法
//////////////////////////////////////////////////////////////////////////

void SHA1_M(const unsigned char *pBytes, const size_t size, unsigned char *pMd){
	SHA1(pBytes, size, pMd);
}

void CRC32(const unsigned char *pBytes, const size_t size, unsigned char *pMd){
	boost::crc_32_type crc32;
	crc32.process_block(pBytes, pBytes + size);

	unsigned int md = crc32.checksum();
	pMd[0] = md >> 24;
	pMd[1] = md >> 16 & 0xFF;
	pMd[2] = md >> 8 & 0xFF;
	pMd[3] = md & 0xFF;
}
