#include "DnsHeader.h"

#include <Windows.h>

#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

 class SockInitializer{
     public:
         SockInitializer()
         {
             WORD versionRequired = MAKEWORD(2, 2);
             WSADATA wsaData;
             int ret = WSAStartup(versionRequired, &wsaData);
             if (0 != ret){
                 std::cerr << "socket initialize failed";
                 exit(1);
             }
         }
         ~SockInitializer(){
             WSACleanup();
         }
 } g_sockIniter;
void err_exit(char* msg){
    cerr << msg << ", errcode:" << WSAGetLastError() << endl;
    exit(1);
}

int main(int argc, char *argv[]){
    cout << sizeof DnsHeader << endl;
    DnsHeader dh;
    dh.ID = rand() % 65535;
    dh.QR = 0;
    dh.OPCODE = 0;
    dh.AA = 0;
    dh.TC = 0;
    dh.RD = 1;
    dh.RA = 0;
    dh.ZERO = 0;
    dh.RCODE = 0;
    dh.QUESTION_COUNT = 1;
    dh.ANSWER_COUNT = 0;
    dh.AUTHORITION_COUNT = 0;
    dh.EXTRA_COUNT = 0;

    DnsQuestion dq;
    dq.QueryName = "search.kuwo.cn";
    dq.QueryClass = 1;
    dq.QueryType = 1;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == sock){
        err_exit("socket call faild");
    }

    struct sockaddr_in dnsaddr;
    dnsaddr.sin_family = AF_INET;
    dnsaddr.sin_port = htons(53);
    dnsaddr.sin_addr.s_addr = inet_addr("202.106.0.20");

    unsigned char request[102400];
    unsigned char response[102400];
    int requestLen = 102400;
    dh.pack(request, requestLen);
    int offset = requestLen;
    requestLen = 102400 - requestLen;
    dq.pack(request + offset, requestLen);

    requestLen += offset;
    if (requestLen != sendto(sock, (const char*)request, requestLen, 0, (struct sockaddr *)(&dnsaddr), sizeof dnsaddr) ) {
        err_exit("sendto failed");
    }

    cout << "sendlen: " << requestLen << endl;

    int recvedLen = recvfrom(sock, (char*)response, sizeof response, 0, NULL, NULL);
    cout << "recvlen: " << recvedLen << endl;

    offset = 0;
    int ret = dh.unpack(response, recvedLen, offset);
    offset += ret;
    if (0 == ret){
        err_exit("header unpack failed");
    }
    ret = dq.unpack(response, recvedLen, offset);
    offset += ret;
    if (0 == ret){
        err_exit("question unpack failed");
    }

    DnsResourceRecord drr;
    drr.unpack(response, recvedLen, offset);

    cout << "ip: " << drr.ip() << endl;
}