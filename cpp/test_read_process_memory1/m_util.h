#ifndef M_UTIL_H__
#define M_UTIL_H__

//////////////////////////////////////////////////////////////////////////
// ���ֽ�����ת��Ϊ uint32
unsigned int byteArrtoUint4(const char *arr, const size_t size = 4);

// ���ֽ�����ת��Ϊ�ַ���
std::string byteArrToHexString(char *arr, int len, char *spliter = "");

// 
void CRC32(const unsigned char *pBytes, const size_t size, unsigned char *pMd);
#endif