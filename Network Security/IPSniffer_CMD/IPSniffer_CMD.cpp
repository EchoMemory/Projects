// IPSniffer_CMD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h> 
#include <Ws2tcpip.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "mstcpip.h"

#define DEFAULT_DEST_IP "210.32.200.98"//默认为本机的IP 
#define STATUS_FAILED 0xFFFF//错误返回值
#define MAX_HOSTNAME_LAN 127
#define MAX_PROTO_TEXT_LEN 255
#define MAX_ADDR_LEN 32
#define MAX_PROTO_NUM 12//子协议数量

typedef struct _tagPROTOMAP
{
	int  ProtoNum;
	char ProtoText[MAX_PROTO_TEXT_LEN];
}PROTOMAP;

static PROTOMAP ProtoMap[MAX_PROTO_NUM]=
{
	{ IPPROTO_IP   , "IP "  },
	{ IPPROTO_ICMP , "ICMP" }, 
	{ IPPROTO_IGMP , "IGMP" },
	{ IPPROTO_GGP  , "GGP " }, 
	{ IPPROTO_TCP  , "TCP " }, 
	{ IPPROTO_PUP  , "PUP " }, 
	{ IPPROTO_UDP  , "UDP " }, 
	{ IPPROTO_IDP  , "IDP " }, 
	{ IPPROTO_ND   , "NP "  }, 
	{ IPPROTO_RAW  , "RAW " }, 
	{ IPPROTO_MAX  , "MAX " },
	{ NULL         , ""     }
};


typedef struct _iphdr //定义IP首部 
{ 
	unsigned char h_verlen; //4位首部长度,4位IP版本号 
	unsigned char tos; //8位服务类型TOS 
	unsigned short total_len; //16位总长度（字节） 
	unsigned short ident; //16位标识 
	unsigned short frag_and_flags; //3位标志位 
	unsigned char ttl; //8位生存时间 TTL 
	unsigned char proto; //8位协议 (TCP, UDP 或其他) 
	unsigned short checksum; //16位IP首部校验和 
	unsigned int sourceIP; //32位源IP地址 
	unsigned int GivenIP; //32位目的IP地址 
}IP_HEADER;
typedef struct _icmphdr //定义ICMP报头(回送与或回送响应)
{ 
	unsigned char i_type;//8位类型
	unsigned char i_code; //8位代码 
	unsigned short i_cksum; //16位校验和 
	unsigned short i_id; //识别号（一般用进程号作为识别号） 
	unsigned short i_seq; //报文序列号 
	unsigned int timestamp;//时间戳 
}ICMP_HEADER;

USHORT checksum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	if(size ) 
	{ 
		cksum += *(UCHAR*)buffer; 
	} 
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum); 
}
struct //定义TCP伪首部 
{ 
	unsigned long saddr; //源地址 
	unsigned long daddr; //目的地址 
	char mbz; 
	char ptcl; //协议类型 
	unsigned short tcpl; //TCP长度 
}psd_header; 

typedef struct _tcphdr //定义TCP首部 
{ 
	USHORT th_sport; //16位源端口 
	USHORT th_dport; //16位目的端口 
	unsigned int th_seq; //32位序列号 
	unsigned int th_ack; //32位确认号 
	unsigned char th_lenres;//4位首部长度/6位保留字 
	unsigned char th_flag;//6位标志位 
	USHORT th_win; //16位窗口大小 
	USHORT th_sum; //16位校验和 
	USHORT th_urp; //16位紧急数据偏移量 
}TCP_HEADER;
typedef struct _udphdr //定义UDP报头 
{
	unsigned short uh_sport;//16位源端口
	unsigned short uh_dport;//16位目的端口
	unsigned short uh_len;//16位长度
	unsigned short uh_sum;//16位校验和
}UDP_HEADER;

int DecodeIpPack(char *buf, int iBufSize);
int DecodeTcpPack(char *TcpBuf);
int DecodeUdpPack(char *UdpBuf);
int DecodeIcmpPack(char *IcmpBuf);

//将协议int转为字符串
char* CheckProtocol(int iProtocol)
{
	for(int i=0; i<MAX_PROTO_NUM; i++)
	{
		if(ProtoMap[i].ProtoNum==iProtocol)
		{
			return ProtoMap[i].ProtoText;
		}
	}
	return "";
}

void usage(char *name) 
{ 
	printf("******************************************\n"); 
	printf("Sniffer Info From Given Packets\n"); 
	printf("usage: %s.exe Given_ip  \n",name); 
	printf("******************************************\n"); 
} 

int main(int argc, char **argv)
{ 
	int ErrorCode; 
	int TimeOut=2000,SendSEQ=0; 
	char RecvBuf[128]; // 每个数据包是128个字节
	char GivenIP[16]; // 要嗅探的机器IP
	memset(GivenIP, 0, 4); 
	// 如果通过参数输入IP，则将这个IP赋给GivenIP，否则将DEFAULT_DEST_IP赋给GivenIP
	if(argc < 2) 
	{
		strcpy(GivenIP, DEFAULT_DEST_IP);
		usage(argv[0]);
		return FALSE;
	}
	else
		strcpy(GivenIP, argv[1]);
	// 以下是声明Socket变量和相应的数据结构
	//初始化SOCKET
	WSADATA wsaData;
	SOCKET SockRaw=(SOCKET)NULL; 
	ErrorCode = WSAStartup(MAKEWORD(2, 1), &wsaData);
	SockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	//获取本机IP地址
	char name[MAX_HOSTNAME_LAN];
	ErrorCode = gethostname(name, MAX_HOSTNAME_LAN);
	struct hostent *pHostent;
	pHostent = (struct hostent*)malloc(sizeof(struct hostent));
	pHostent = gethostbyname(name);
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(6000);
	memcpy(&sa.sin_addr.S_un.S_addr, pHostent->h_addr_list[0], pHostent->h_length);
	ErrorCode = bind(SockRaw, (PSOCKADDR) &sa, sizeof(sa));
	//设置SOCK_RAW为SIO_RCVALL，以便接收所有的IP包
	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1;
	DWORD dwBytesReturned = 0;
	ErrorCode = WSAIoctl(SockRaw,SIO_RCVALL,&dwBufferInLen,sizeof(dwBufferInLen),
		&dwBufferLen,sizeof(dwBufferLen),&dwBytesReturned,NULL,NULL);
	//侦听IP报文
	while (1)
	{
		memset(RecvBuf,0,sizeof(RecvBuf));
		ErrorCode = recv(SockRaw, RecvBuf, sizeof(RecvBuf), 0);
		ErrorCode = DecodeIpPack(RecvBuf, ErrorCode);
	}
	closesocket(SockRaw);
	WSACleanup();
	
	return ErrorCode;
}
//IP包解析
int DecodeIpPack(char *buf, int iBufSize)
{
	IP_HEADER *pIpheader;
	int iProtocol, iTTL;
	char szProtocol[MAX_PROTO_TEXT_LEN];
	char szSourceIP[MAX_ADDR_LEN], szGivenIP[MAX_ADDR_LEN];
	SOCKADDR_IN saSource, saDest;
	pIpheader = (IP_HEADER*)buf;
	iProtocol = pIpheader->proto;
	strncpy(szProtocol, CheckProtocol(iProtocol), MAX_PROTO_TEXT_LEN);
	
	//获得源IP地址
	saSource.sin_addr.s_addr = pIpheader->sourceIP;
	strncpy(szSourceIP, inet_ntoa(saSource.sin_addr), MAX_ADDR_LEN);
	//获得目的IP地址
	saDest.sin_addr.s_addr = pIpheader->GivenIP;
	strncpy(szGivenIP, inet_ntoa(saDest.sin_addr), MAX_ADDR_LEN);
	iTTL = pIpheader->ttl;
	//打印
	printf("%s ", szProtocol);
	printf("%s->%s ", szSourceIP, szGivenIP);
	printf("bytes=%d TTL=%d ", iBufSize, iTTL);
	//计算IP包的首部长度
	int iIphLen = sizeof(unsigned long)*(pIpheader->h_verlen &0xf);
	//协议解析
	switch (iProtocol)
	{
	case IPPROTO_TCP:
		DecodeTcpPack(buf + iIphLen);
		break;
	case IPPROTO_UDP:
		DecodeUdpPack(buf + iIphLen);
		break;
	case IPPROTO_ICMP:
		DecodeIcmpPack(buf + iIphLen);
		break;
	default:
		break;
	}
	return true;
}
//TCP报文解析
int DecodeTcpPack(char *TcpBuf)
{
	TCP_HEADER *pTcpHeader;
	int i;
	pTcpHeader = (TCP_HEADER*)TcpBuf;
	printf("Port:%d->%d ", ntohs(pTcpHeader->th_sport), ntohs(pTcpHeader->th_dport));
	unsigned char FlagMask = 1;
	for (i = 0; i < 6; i++)
	{
		if ((pTcpHeader->th_flag) &FlagMask)
			printf("%c", ((pTcpHeader->th_flag))<<(i+2) & 0x10);
		else
			printf("-");
		FlagMask = FlagMask << 1;
	}
	printf("\n");
	return true;
} 
//UDP报文解析
int DecodeUdpPack(char *UdpBuf)
{
	UDP_HEADER *pUdpHeader;
	pUdpHeader = (UDP_HEADER*)UdpBuf;
	printf("Port:%d->%d ", ntohs(pUdpHeader->uh_sport), ntohs(pUdpHeader->uh_dport));
	printf("Len=%d\n", ntohs(pUdpHeader->uh_len));
	return true;
}
//ICMP报文解析 
int DecodeIcmpPack(char *IcmpBuf)
{
	ICMP_HEADER *pIcmpHeader;
	pIcmpHeader = (ICMP_HEADER*)IcmpBuf;
	printf("Type:%d,Code:%d ", pIcmpHeader->i_type, pIcmpHeader->i_code);
	printf("ID=%d SEQ=%d\n", pIcmpHeader->i_id, pIcmpHeader->i_seq);
	return true;
}


