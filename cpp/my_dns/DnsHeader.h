#ifndef DNS_HEADER_H__
#define DNS_HEADER_H__

#include <string>
#include <vector>

// TCP/IP详解 14.3
#define DNS_HEADER_SIZE 12

#include <PshPack1.h>
#ifdef BIG_ENDIAN
struct DnsHeader{
    // id
    unsigned ID:16;
    // flag, 2字节
    unsigned QR:1;     // 0:查询报文, 1响应报文
    unsigned OPCODE:4; // 0:标准查询, 1:反向查询, 2服务器请求状态
    unsigned AA:1;     // authoritative answer
    unsigned TC:1;     // truncated, 使用UDP时，表示应答的总长度超过512字节时，只返回前512字节
    unsigned RD:1;     // recurision desired(期望递归)
    unsigned RA:1;     // 可用递归，如果服务器支持递归查询，则在此响应中将该比特设置为1
    unsigned ZERO:3;   // unused, 必须设为0
    unsigned RCODE:4;  // 返回码，0:没有差错，3:名字差错
    // 问题数
    unsigned QUESTION_COUNT:16;
    // 资源记录数
    unsigned ANSWER_COUNT:16;
    // 授权资源记录数
    unsigned AUTHORITION_COUNT:16; // zero
    // 额外资源记录数
    unsigned EXTRA_COUNT:16; // zero
};
#elif defined(LITTLE_ENDIAN)
struct DnsHeader{
    // id
    unsigned ID:16;
    // flag, 2字节
    unsigned RD:1;     // recurision desired(期望递归)
    unsigned TC:1;     // truncated, 使用UDP时，表示应答的总长度超过512字节时，只返回前512字节
    unsigned AA:1;     // authoritative answer
    unsigned OPCODE:4; // 0:标准查询, 1:反向查询, 2服务器请求状态
    unsigned QR:1;     // 0:查询报文, 1响应报文

    unsigned RCODE:4;  // 返回码，0:没有差错，3:名字差错
    unsigned ZERO:3;   // unused, 必须设为0
    unsigned RA:1;     // 可用递归，如果服务器支持递归查询，则在此响应中将该比特设置为1

    // 问题数
    unsigned QUESTION_COUNT:16;
    // 资源记录数
    unsigned ANSWER_COUNT:16;
    // 授权资源记录数
    unsigned AUTHORITION_COUNT:16; // zero
    // 额外资源记录数
    unsigned EXTRA_COUNT:16; // zero
};
#else
struct DnsHeader{
    // id
    unsigned short ID;
    // flag, 2字节
    unsigned char QR;     // 1bit, 0:查询报文, 1响应报文
    unsigned char OPCODE; // 4bit, 0:标准查询, 1:反向查询, 2服务器请求状态
    unsigned char AA;     // 1bit, authoritative answer
    unsigned char TC;     // 1bit, truncated, 使用UDP时，表示应答的总长度超过512字节时，只返回前512字节
    unsigned char RD;     // 1bit, recurision desired(期望递归)
    unsigned char RA;     // 1bit, 可用递归，如果服务器支持递归查询，则在此响应中将该比特设置为1
    unsigned char ZERO;   // 3bit, unused, 必须设为0
    unsigned char RCODE;  // 4bit, 返回码，0:没有差错，3:名字差错
    // 问题数
    unsigned short QUESTION_COUNT;
    // 资源记录数
    unsigned short  ANSWER_COUNT;
    // 授权资源记录数
    unsigned short AUTHORITION_COUNT; // zero
    // 额外资源记录数
    unsigned short EXTRA_COUNT; // zero

public:
    int pack(unsigned char *buf, int &bufsize);
    int unpack(const unsigned char *buf, int bufsize, int offset);
};

#endif
#include <PopPack.h>

struct DnsQuestion{
    std::string QueryName;
    unsigned short QueryType;  // 查询类型
    unsigned short QueryClass; // 查询类

public:
    int pack(unsigned char *buf, int &bufsize);
    int unpack(const unsigned char *buf, int bufsize, int offset);

};

struct DnsResourceRecord{
    std::string DomainName;
    unsigned short Type;
    unsigned short Class;
    unsigned long   TTL;
    unsigned short DataLength;   
    unsigned char Data[256];

    int unpack(const unsigned char *buf, int bufsize, int offset);
    std::string ip();
};

struct DnsRequest{
    DnsHeader header;
    DnsQuestion question;
};

struct DnsResponse{
    DnsHeader header;
    DnsQuestion question;
    std::vector<DnsResourceRecord> answer;
};

#endif