// IPSniffer_CMD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h> 
#include <Ws2tcpip.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "mstcpip.h"

#define DEFAULT_DEST_IP "210.32.200.98"//Ĭ��Ϊ������IP 
#define STATUS_FAILED 0xFFFF//���󷵻�ֵ
#define MAX_HOSTNAME_LAN 127
#define MAX_PROTO_TEXT_LEN 255
#define MAX_ADDR_LEN 32
#define MAX_PROTO_NUM 12//��Э������

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


typedef struct _iphdr //����IP�ײ� 
{ 
	unsigned char h_verlen; //4λ�ײ�����,4λIP�汾�� 
	unsigned char tos; //8λ��������TOS 
	unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ� 
	unsigned short ident; //16λ��ʶ 
	unsigned short frag_and_flags; //3λ��־λ 
	unsigned char ttl; //8λ����ʱ�� TTL 
	unsigned char proto; //8λЭ�� (TCP, UDP ������) 
	unsigned short checksum; //16λIP�ײ�У��� 
	unsigned int sourceIP; //32λԴIP��ַ 
	unsigned int GivenIP; //32λĿ��IP��ַ 
}IP_HEADER;
typedef struct _icmphdr //����ICMP��ͷ(������������Ӧ)
{ 
	unsigned char i_type;//8λ����
	unsigned char i_code; //8λ���� 
	unsigned short i_cksum; //16λУ��� 
	unsigned short i_id; //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ� 
	unsigned short i_seq; //�������к� 
	unsigned int timestamp;//ʱ��� 
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
struct //����TCPα�ײ� 
{ 
	unsigned long saddr; //Դ��ַ 
	unsigned long daddr; //Ŀ�ĵ�ַ 
	char mbz; 
	char ptcl; //Э������ 
	unsigned short tcpl; //TCP���� 
}psd_header; 

typedef struct _tcphdr //����TCP�ײ� 
{ 
	USHORT th_sport; //16λԴ�˿� 
	USHORT th_dport; //16λĿ�Ķ˿� 
	unsigned int th_seq; //32λ���к� 
	unsigned int th_ack; //32λȷ�Ϻ� 
	unsigned char th_lenres;//4λ�ײ�����/6λ������ 
	unsigned char th_flag;//6λ��־λ 
	USHORT th_win; //16λ���ڴ�С 
	USHORT th_sum; //16λУ��� 
	USHORT th_urp; //16λ��������ƫ���� 
}TCP_HEADER;
typedef struct _udphdr //����UDP��ͷ 
{
	unsigned short uh_sport;//16λԴ�˿�
	unsigned short uh_dport;//16λĿ�Ķ˿�
	unsigned short uh_len;//16λ����
	unsigned short uh_sum;//16λУ���
}UDP_HEADER;

int DecodeIpPack(char *buf, int iBufSize);
int DecodeTcpPack(char *TcpBuf);
int DecodeUdpPack(char *UdpBuf);
int DecodeIcmpPack(char *IcmpBuf);

//��Э��intתΪ�ַ���
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
	char RecvBuf[128]; // ÿ�����ݰ���128���ֽ�
	char GivenIP[16]; // Ҫ��̽�Ļ���IP
	memset(GivenIP, 0, 4); 
	// ���ͨ����������IP�������IP����GivenIP������DEFAULT_DEST_IP����GivenIP
	if(argc < 2) 
	{
		strcpy(GivenIP, DEFAULT_DEST_IP);
		usage(argv[0]);
		return FALSE;
	}
	else
		strcpy(GivenIP, argv[1]);
	// ����������Socket��������Ӧ�����ݽṹ
	//��ʼ��SOCKET
	WSADATA wsaData;
	SOCKET SockRaw=(SOCKET)NULL; 
	ErrorCode = WSAStartup(MAKEWORD(2, 1), &wsaData);
	SockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	//��ȡ����IP��ַ
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
	//����SOCK_RAWΪSIO_RCVALL���Ա�������е�IP��
	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1;
	DWORD dwBytesReturned = 0;
	ErrorCode = WSAIoctl(SockRaw,SIO_RCVALL,&dwBufferInLen,sizeof(dwBufferInLen),
		&dwBufferLen,sizeof(dwBufferLen),&dwBytesReturned,NULL,NULL);
	//����IP����
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
//IP������
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
	
	//���ԴIP��ַ
	saSource.sin_addr.s_addr = pIpheader->sourceIP;
	strncpy(szSourceIP, inet_ntoa(saSource.sin_addr), MAX_ADDR_LEN);
	//���Ŀ��IP��ַ
	saDest.sin_addr.s_addr = pIpheader->GivenIP;
	strncpy(szGivenIP, inet_ntoa(saDest.sin_addr), MAX_ADDR_LEN);
	iTTL = pIpheader->ttl;
	//��ӡ
	printf("%s ", szProtocol);
	printf("%s->%s ", szSourceIP, szGivenIP);
	printf("bytes=%d TTL=%d ", iBufSize, iTTL);
	//����IP�����ײ�����
	int iIphLen = sizeof(unsigned long)*(pIpheader->h_verlen &0xf);
	//Э�����
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
//TCP���Ľ���
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
//UDP���Ľ���
int DecodeUdpPack(char *UdpBuf)
{
	UDP_HEADER *pUdpHeader;
	pUdpHeader = (UDP_HEADER*)UdpBuf;
	printf("Port:%d->%d ", ntohs(pUdpHeader->uh_sport), ntohs(pUdpHeader->uh_dport));
	printf("Len=%d\n", ntohs(pUdpHeader->uh_len));
	return true;
}
//ICMP���Ľ��� 
int DecodeIcmpPack(char *IcmpBuf)
{
	ICMP_HEADER *pIcmpHeader;
	pIcmpHeader = (ICMP_HEADER*)IcmpBuf;
	printf("Type:%d,Code:%d ", pIcmpHeader->i_type, pIcmpHeader->i_code);
	printf("ID=%d SEQ=%d\n", pIcmpHeader->i_id, pIcmpHeader->i_seq);
	return true;
}


