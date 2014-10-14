#include "DnsHeader.h"

#include <Windows.h>

#include <cstring>
#include <sstream>

int DnsHeader::pack(unsigned char *buf, int &bufsize){
    int tmpBufSize = bufsize;
    bufsize = DNS_HEADER_SIZE;

    if (tmpBufSize < DNS_HEADER_SIZE) return 0;

    // id
    unsigned netShort = htons(ID);
    memcpy(buf, &netShort, 2);
    buf += 2;

    // flag
    unsigned char leftFlag = QR;
    leftFlag = (leftFlag << 4) | OPCODE;
    leftFlag = (leftFlag << 1) | AA;
    leftFlag = (leftFlag << 1) | TC;
    leftFlag = (leftFlag << 1) | RD;

    memcpy(buf, &leftFlag, 1);
    buf += 1;

    unsigned char rightFlag = RA;
    rightFlag = (rightFlag << 7) | RCODE;

    memcpy(buf, &rightFlag, 1);
    buf += 1;

    // 问题数
    netShort = htons(QUESTION_COUNT);
    memcpy(buf, &netShort, 2);
    buf += 2;

    // 资源记录数
    netShort = htons(ANSWER_COUNT);
    memcpy(buf, &netShort, 2);
    buf += 2;

    // 授权资源记录数
    netShort = htons(AUTHORITION_COUNT);
    memcpy(buf, &netShort, 2);
    buf += 2;

    // 额外资源记录数
    netShort = htons(EXTRA_COUNT);
    memcpy(buf, &netShort, 2);

    // the end
    return DNS_HEADER_SIZE;
}


int DnsHeader::unpack(const unsigned char *buf, int bufsize, int offset){
    if (bufsize - offset < DNS_HEADER_SIZE)  return 0;

    buf += offset;

    // id
    ID = ntohs(*reinterpret_cast<const short *>(buf));
    buf += 2;

    // flag
    unsigned char leftFlag = *buf;
    QR = leftFlag >> 7;
    OPCODE = (leftFlag & 0x7f) >> 3;
    AA = (leftFlag & 0x07) >> 2;
    TC = (leftFlag & 0x03) >> 1;
    RD = leftFlag & 0x01;

    buf += 1;

    unsigned char rightFlag = *buf;
    RA = rightFlag >> 7;
    ZERO = 0;
    RCODE = rightFlag & 0x0f;

    buf += 1;

    // 问题数
    QUESTION_COUNT = ntohs(*reinterpret_cast<const short *>(buf));
    buf += 2;

    ANSWER_COUNT = ntohs(*reinterpret_cast<const short*>(buf));
    buf += 2;

    AUTHORITION_COUNT = ntohs(*reinterpret_cast<const short*>(buf));
    buf += 2;

    EXTRA_COUNT = ntohs(*reinterpret_cast<const short*>(buf));

    // the end
    return DNS_HEADER_SIZE;
}

int DnsQuestion::pack( unsigned char *buf, int &bufsize )
{
    int tmpBufsize = bufsize;
    bufsize = QueryName.size() + 2 + 4;
    if (tmpBufsize < bufsize) return 0;

    // 查询名
    const int query_name_count = QueryName.size();
    int offset = 0;
    int tmpoffset;
    while (offset != std::string::npos && offset < query_name_count){
        tmpoffset = QueryName.find('.', offset);
        if (tmpoffset != std::string::npos){
            const int sublen = tmpoffset - offset;
            *buf = static_cast<unsigned char>(sublen);
            buf += 1;
            memcpy(buf, QueryName.substr(offset, sublen).c_str(), sublen);
            buf += sublen;

            offset = tmpoffset + 1;
        }
        else{
            const int sublen = query_name_count - offset;
            *buf = static_cast<unsigned char>(sublen);
            buf += 1;
            memcpy(buf, QueryName.substr(offset, sublen).c_str(), sublen);
            buf += sublen;
            *buf = 0;
            buf += 1;

            break;
        }
    }

    // 查询类型
    unsigned short netShort = htons(QueryType);
    memcpy(buf, &netShort, 2);
    buf += 2;

    netShort = htons(QueryClass);
    memcpy(buf, &netShort, 2);

    // the end

    return bufsize;
}

int unpackDomainName( const unsigned char *buf, int bufsize, int offset, std::string &domainName ){
    const int bakOffset = offset;
    domainName.clear();

    int result = 0;
    if ( (buf[offset] & 0xc0) == 0xc0){
        int domainOffset = (buf[offset] & 0x3f) | buf[offset + 1];
        unpackDomainName(buf, bufsize, domainOffset, domainName);

        result = 2;
    }
    else{
        int sublen;
        while (sublen = *(buf + offset) ){
            offset += 1;

            if (bufsize < offset + sublen + 1) return 0;

            if (!domainName.empty()) domainName.append(".");
            domainName.append(buf + offset, buf + offset + sublen);
            offset += sublen;
        }

        offset += 1;

        result = offset - bakOffset;

    }
    return result;
}


int DnsQuestion::unpack( const unsigned char *buf, int bufsize, int offset )
{
    const int bakOffset = offset;
    QueryType = 0;
    QueryClass = 0;
    QueryName.clear();

    // 查询名

    int domainLen = unpackDomainName(buf, bufsize, offset ,QueryName);
    if (!domainLen) return 0;

    offset += domainLen;

    if (bufsize < offset + 4) return 0;

    QueryType = ntohs(*reinterpret_cast<const unsigned short*>(buf + offset));
    offset += 2;
    QueryClass = ntohs(*reinterpret_cast<const unsigned short*>(buf + offset));
    offset += 2;

    return offset - bakOffset;

}

int DnsResourceRecord::unpack( const unsigned char *buf, int bufsize, int offset )
{
    const int bakOffset = offset;
    // 查询名
    int domainLen = unpackDomainName(buf, bufsize, offset, DomainName);
    if (!domainLen) return 0;

    offset += domainLen;

    if (bufsize < offset + 8) return 0;
    Type = ntohs(*reinterpret_cast<const unsigned short*>(buf + offset));
    offset += 2;
    Class = ntohs(*reinterpret_cast<const unsigned short*>(buf + offset));
    offset += 2;
    TTL = ntohl(*reinterpret_cast<const unsigned long*>(buf + offset));
    offset += 4;
    DataLength = ntohs(*reinterpret_cast<const unsigned short*>(buf + offset));
    offset += 2;
    if (bufsize < offset + DataLength || DataLength > sizeof Data) return 0;
    memcpy(Data, buf + offset, DataLength);

    offset += DataLength;

    return offset - bakOffset;
}

std::string DnsResourceRecord::ip()
{
    if (DataLength != 4) return std::string();
    std::stringstream ss;
    ss << (int)Data[0] << "." << (int)Data[1] << "." << (int)Data[2] << "." << (int)Data[3];

    return ss.str();
}
