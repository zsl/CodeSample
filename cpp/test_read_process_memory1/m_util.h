#ifndef M_UTIL_H__
#define M_UTIL_H__

//////////////////////////////////////////////////////////////////////////
// 将字节数组转化为 uint32
unsigned int byteArrtoUint4(const char *arr, const size_t size = 4);

// 将字节数组转换为字符串
std::string byteArrToHexString(char *arr, int len, char *spliter = "");

// 
void CRC32(const unsigned char *pBytes, const size_t size, unsigned char *pMd);
#endif