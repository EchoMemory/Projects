//////////////////////////////////////////////////
// protoinfo.h�ļ�

/*

����Э���ʽ
����Э����ʹ�õĺ�

 */


#ifndef __PROTOINFO_H__
#define __PROTOINFO_H__


#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_ARP   0x0806
#define MAX_HOSTNAME_LAN 127
#define MAX_PROTO_TEXT_LEN 255
#define MAX_ADDR_LEN 32
#define MAX_PROTO_NUM 12			//��Э������

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

typedef struct _ETHeader         // 14�ֽڵ���̫ͷ
{
	UCHAR	dhost[6];			// Ŀ��MAC��ַdestination mac address
	UCHAR	shost[6];			// ԴMAC��ַsource mac address
	USHORT	type;				// �²�Э�����ͣ���IP��ETHERTYPE_IP����ARP��ETHERTYPE_ARP����
} ETHeader, *PETHeader;


#define ARPHRD_ETHER 	1

// ARPЭ��opcodes
#define	ARPOP_REQUEST	1		// ARP ����	
#define	ARPOP_REPLY		2		// ARP ��Ӧ


typedef struct _ARPHeader		// 28�ֽڵ�ARPͷ
{
	USHORT	hrd;				//	Ӳ����ַ�ռ䣬��̫����ΪARPHRD_ETHER
	USHORT	eth_type;			//  ��̫�����ͣ�ETHERTYPE_IP ����
	UCHAR	maclen;				//	MAC��ַ�ĳ��ȣ�Ϊ6
	UCHAR	iplen;				//	IP��ַ�ĳ��ȣ�Ϊ4
	USHORT	opcode;				//	�������룬ARPOP_REQUESTΪ����ARPOP_REPLYΪ��Ӧ
	UCHAR	smac[6];			//	ԴMAC��ַ
	UCHAR	saddr[4];			//	ԴIP��ַ
	UCHAR	dmac[6];			//	Ŀ��MAC��ַ
	UCHAR	daddr[4];			//	Ŀ��IP��ַ
} ARPHeader, *PARPHeader;


// Э��
#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20�ֽڵ�IPͷ
{
    UCHAR     iphVerLen;      // �汾�ź�ͷ���ȣ���ռ4λ��
    UCHAR     ipTOS;          // �������� 
    USHORT    ipLength;       // ����ܳ��ȣ�������IP���ĳ���
    USHORT    ipID;			  // �����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�
    USHORT    ipFlags;	      // ��־
    UCHAR     ipTTL;	      // ����ʱ�䣬����TTL
    UCHAR     ipProtocol;     // Э�飬������TCP��UDP��ICMP��
    USHORT    ipChecksum;     // У���
    ULONG     ipSource;       // ԴIP��ַ
    ULONG     ipDestination;  // Ŀ��IP��ַ
} IPHeader, *PIPHeader; 


// ����TCP��־
#define   TCP_FIN   0x01
#define   TCP_SYN   0x02
#define   TCP_RST   0x04
#define   TCP_PSH   0x08
#define   TCP_ACK   0x10
#define   TCP_URG   0x20
#define   TCP_ACE   0x40
#define   TCP_CWR   0x80

typedef struct _TCPHeader		// 20�ֽڵ�TCPͷ
{
	USHORT	sourcePort;			// 16λԴ�˿ں�
	USHORT	destinationPort;	// 16λĿ�Ķ˿ں�
	ULONG	sequenceNumber;		// 32λ���к�
	ULONG	acknowledgeNumber;	// 32λȷ�Ϻ�
	UCHAR	dataoffset;			// ��4λ��ʾ����ƫ��
	UCHAR	flags;				// 6λ��־λ
								//FIN - 0x01
								//SYN - 0x02
								//RST - 0x04 
								//PUSH- 0x08
								//ACK- 0x10
								//URG- 0x20
								//ACE- 0x40
								//CWR- 0x80

	USHORT	windows;			// 16λ���ڴ�С
	USHORT	checksum;			// 16λУ���
	USHORT	urgentPointer;		// 16λ��������ƫ���� 
} TCPHeader, *PTCPHeader;

typedef struct psd_hdr //����TCPα��ͷ 
{ 
	unsigned long saddr; //Դ��ַ 
	unsigned long daddr; //Ŀ�ĵ�ַ 
	char mbz; 
	char ptcl; //Э������ 
	unsigned short tcpl; //TCP���� 
}PSD_HEADER;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// Դ�˿ں�		
	USHORT			destinationPort;// Ŀ�Ķ˿ں�		
	USHORT			len;			// �������
	USHORT			checksum;		// У���
} UDPHeader, *PUDPHeader;

typedef struct _ICMPHeader //����ICMP��ͷ(������������Ӧ)
{ 
	unsigned char i_type;//8λ����
	unsigned char i_code; //8λ���� 
	unsigned short i_cksum; //16λУ��� 
	unsigned short i_id; //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ� 
	unsigned short i_seq; //�������к� 
	unsigned int timestamp;//ʱ��� 
}ICMP_HEADER;

#endif // __PROTOINFO_H__

