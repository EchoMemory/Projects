//////////////////////////////////////////////////
// protoinfo.h文件

/*

定义协议格式
定义协议中使用的宏

 */


#ifndef __PROTOINFO_H__
#define __PROTOINFO_H__


#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_ARP   0x0806
#define MAX_HOSTNAME_LAN 127
#define MAX_PROTO_TEXT_LEN 255
#define MAX_ADDR_LEN 32
#define MAX_PROTO_NUM 12			//子协议数量

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

typedef struct _ETHeader         // 14字节的以太头
{
	UCHAR	dhost[6];			// 目的MAC地址destination mac address
	UCHAR	shost[6];			// 源MAC地址source mac address
	USHORT	type;				// 下层协议类型，如IP（ETHERTYPE_IP）、ARP（ETHERTYPE_ARP）等
} ETHeader, *PETHeader;


#define ARPHRD_ETHER 	1

// ARP协议opcodes
#define	ARPOP_REQUEST	1		// ARP 请求	
#define	ARPOP_REPLY		2		// ARP 响应


typedef struct _ARPHeader		// 28字节的ARP头
{
	USHORT	hrd;				//	硬件地址空间，以太网中为ARPHRD_ETHER
	USHORT	eth_type;			//  以太网类型，ETHERTYPE_IP ？？
	UCHAR	maclen;				//	MAC地址的长度，为6
	UCHAR	iplen;				//	IP地址的长度，为4
	USHORT	opcode;				//	操作代码，ARPOP_REQUEST为请求，ARPOP_REPLY为响应
	UCHAR	smac[6];			//	源MAC地址
	UCHAR	saddr[4];			//	源IP地址
	UCHAR	dmac[6];			//	目的MAC地址
	UCHAR	daddr[4];			//	目的IP地址
} ARPHeader, *PARPHeader;


// 协议
#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20字节的IP头
{
    UCHAR     iphVerLen;      // 版本号和头长度（各占4位）
    UCHAR     ipTOS;          // 服务类型 
    USHORT    ipLength;       // 封包总长度，即整个IP报的长度
    USHORT    ipID;			  // 封包标识，惟一标识发送的每一个数据报
    USHORT    ipFlags;	      // 标志
    UCHAR     ipTTL;	      // 生存时间，就是TTL
    UCHAR     ipProtocol;     // 协议，可能是TCP、UDP、ICMP等
    USHORT    ipChecksum;     // 校验和
    ULONG     ipSource;       // 源IP地址
    ULONG     ipDestination;  // 目标IP地址
} IPHeader, *PIPHeader; 


// 定义TCP标志
#define   TCP_FIN   0x01
#define   TCP_SYN   0x02
#define   TCP_RST   0x04
#define   TCP_PSH   0x08
#define   TCP_ACK   0x10
#define   TCP_URG   0x20
#define   TCP_ACE   0x40
#define   TCP_CWR   0x80

typedef struct _TCPHeader		// 20字节的TCP头
{
	USHORT	sourcePort;			// 16位源端口号
	USHORT	destinationPort;	// 16位目的端口号
	ULONG	sequenceNumber;		// 32位序列号
	ULONG	acknowledgeNumber;	// 32位确认号
	UCHAR	dataoffset;			// 高4位表示数据偏移
	UCHAR	flags;				// 6位标志位
								//FIN - 0x01
								//SYN - 0x02
								//RST - 0x04 
								//PUSH- 0x08
								//ACK- 0x10
								//URG- 0x20
								//ACE- 0x40
								//CWR- 0x80

	USHORT	windows;			// 16位窗口大小
	USHORT	checksum;			// 16位校验和
	USHORT	urgentPointer;		// 16位紧急数据偏移量 
} TCPHeader, *PTCPHeader;

typedef struct psd_hdr //定义TCP伪报头 
{ 
	unsigned long saddr; //源地址 
	unsigned long daddr; //目的地址 
	char mbz; 
	char ptcl; //协议类型 
	unsigned short tcpl; //TCP长度 
}PSD_HEADER;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// 源端口号		
	USHORT			destinationPort;// 目的端口号		
	USHORT			len;			// 封包长度
	USHORT			checksum;		// 校验和
} UDPHeader, *PUDPHeader;

typedef struct _ICMPHeader //定义ICMP报头(回送与或回送响应)
{ 
	unsigned char i_type;//8位类型
	unsigned char i_code; //8位代码 
	unsigned short i_cksum; //16位校验和 
	unsigned short i_id; //识别号（一般用进程号作为识别号） 
	unsigned short i_seq; //报文序列号 
	unsigned int timestamp;//时间戳 
}ICMP_HEADER;

#endif // __PROTOINFO_H__

