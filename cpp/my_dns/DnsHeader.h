#ifndef DNS_HEADER_H__
#define DNS_HEADER_H__

#include <string>
#include <vector>

// TCP/IP��� 14.3
#define DNS_HEADER_SIZE 12

#include <PshPack1.h>
#ifdef BIG_ENDIAN
struct DnsHeader{
    // id
    unsigned ID:16;
    // flag, 2�ֽ�
    unsigned QR:1;     // 0:��ѯ����, 1��Ӧ����
    unsigned OPCODE:4; // 0:��׼��ѯ, 1:�����ѯ, 2����������״̬
    unsigned AA:1;     // authoritative answer
    unsigned TC:1;     // truncated, ʹ��UDPʱ����ʾӦ����ܳ��ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽ�
    unsigned RD:1;     // recurision desired(�����ݹ�)
    unsigned RA:1;     // ���õݹ飬���������֧�ֵݹ��ѯ�����ڴ���Ӧ�н��ñ�������Ϊ1
    unsigned ZERO:3;   // unused, ������Ϊ0
    unsigned RCODE:4;  // �����룬0:û�в��3:���ֲ��
    // ������
    unsigned QUESTION_COUNT:16;
    // ��Դ��¼��
    unsigned ANSWER_COUNT:16;
    // ��Ȩ��Դ��¼��
    unsigned AUTHORITION_COUNT:16; // zero
    // ������Դ��¼��
    unsigned EXTRA_COUNT:16; // zero
};
#elif defined(LITTLE_ENDIAN)
struct DnsHeader{
    // id
    unsigned ID:16;
    // flag, 2�ֽ�
    unsigned RD:1;     // recurision desired(�����ݹ�)
    unsigned TC:1;     // truncated, ʹ��UDPʱ����ʾӦ����ܳ��ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽ�
    unsigned AA:1;     // authoritative answer
    unsigned OPCODE:4; // 0:��׼��ѯ, 1:�����ѯ, 2����������״̬
    unsigned QR:1;     // 0:��ѯ����, 1��Ӧ����

    unsigned RCODE:4;  // �����룬0:û�в��3:���ֲ��
    unsigned ZERO:3;   // unused, ������Ϊ0
    unsigned RA:1;     // ���õݹ飬���������֧�ֵݹ��ѯ�����ڴ���Ӧ�н��ñ�������Ϊ1

    // ������
    unsigned QUESTION_COUNT:16;
    // ��Դ��¼��
    unsigned ANSWER_COUNT:16;
    // ��Ȩ��Դ��¼��
    unsigned AUTHORITION_COUNT:16; // zero
    // ������Դ��¼��
    unsigned EXTRA_COUNT:16; // zero
};
#else
struct DnsHeader{
    // id
    unsigned short ID;
    // flag, 2�ֽ�
    unsigned char QR;     // 1bit, 0:��ѯ����, 1��Ӧ����
    unsigned char OPCODE; // 4bit, 0:��׼��ѯ, 1:�����ѯ, 2����������״̬
    unsigned char AA;     // 1bit, authoritative answer
    unsigned char TC;     // 1bit, truncated, ʹ��UDPʱ����ʾӦ����ܳ��ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽ�
    unsigned char RD;     // 1bit, recurision desired(�����ݹ�)
    unsigned char RA;     // 1bit, ���õݹ飬���������֧�ֵݹ��ѯ�����ڴ���Ӧ�н��ñ�������Ϊ1
    unsigned char ZERO;   // 3bit, unused, ������Ϊ0
    unsigned char RCODE;  // 4bit, �����룬0:û�в��3:���ֲ��
    // ������
    unsigned short QUESTION_COUNT;
    // ��Դ��¼��
    unsigned short  ANSWER_COUNT;
    // ��Ȩ��Դ��¼��
    unsigned short AUTHORITION_COUNT; // zero
    // ������Դ��¼��
    unsigned short EXTRA_COUNT; // zero

public:
    int pack(unsigned char *buf, int &bufsize);
    int unpack(const unsigned char *buf, int bufsize, int offset);
};

#endif
#include <PopPack.h>

struct DnsQuestion{
    std::string QueryName;
    unsigned short QueryType;  // ��ѯ����
    unsigned short QueryClass; // ��ѯ��

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