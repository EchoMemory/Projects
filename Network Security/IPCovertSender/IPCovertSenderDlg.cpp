// IPCovertSenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IPCovertSender.h"
#include "IPCovertSenderDlg.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInitSock theSock;
UINT filebuff[1024] = { 0 };
int num_of_packet;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCovertSenderDlg dialog

CIPCovertSenderDlg::CIPCovertSenderDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIPCovertSenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIPCovertSenderDlg)
	m_CheckSum = 0;
	m_FragmentationType = 0;
	m_HeaderSize = 0;
	m_Identification = 0;
	m_Offset = 0;
	m_Protocol = 0;
	m_TotalLenth = 0;
	m_TypeOfService = 0;
	m_Version = 0;
	m_TTL = 0;
	m_ICMP_Checksum = 0;
	m_ICMP_Code = 0;
	m_ICMP_Type = 0;
	m_ICMP_ID = 0;
	m_ICMP_Sequence = 0;
	m_ICMP_Timestamp = 0;
	m_sFilePathName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPCovertSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPCovertSenderDlg)
	DDX_Control(pDX, IDC_SrcAddress, m_SrcAddress);
	DDX_Control(pDX, IDC_DestAddress, m_DestAddress);
	DDX_Text(pDX, IDC_CheckSum, m_CheckSum);
	DDV_MinMaxUInt(pDX, m_CheckSum, 0, 65535);
	DDX_Text(pDX, IDC_FragmentationType, m_FragmentationType);
	DDV_MinMaxUInt(pDX, m_FragmentationType, 0, 7);
	DDX_Text(pDX, IDC_HeaderSize, m_HeaderSize);
	DDV_MinMaxByte(pDX, m_HeaderSize, 5, 15);
	DDX_Text(pDX, IDC_Identification, m_Identification);
	DDV_MinMaxUInt(pDX, m_Identification, 0, 65535);
	DDX_Text(pDX, IDC_Offset, m_Offset);
	DDV_MinMaxUInt(pDX, m_Offset, 0, 8191);
	DDX_Text(pDX, IDC_Protocol, m_Protocol);
	DDV_MinMaxByte(pDX, m_Protocol, 0, 255);
	DDX_Text(pDX, IDC_TotalLenth, m_TotalLenth);
	DDV_MinMaxUInt(pDX, m_TotalLenth, 0, 65535);
	DDX_Text(pDX, IDC_TypeOfService, m_TypeOfService);
	DDV_MinMaxByte(pDX, m_TypeOfService, 0, 30);
	DDX_Text(pDX, IDC_Version, m_Version);
	DDV_MinMaxByte(pDX, m_Version, 4, 6);
	DDX_Text(pDX, IDC_TTL, m_TTL);
	DDV_MinMaxByte(pDX, m_TTL, 0, 255);
	DDX_Text(pDX, IDC_ICMP_Checksum, m_ICMP_Checksum);
	DDV_MinMaxUInt(pDX, m_ICMP_Checksum, 0, 65535);
	DDX_Text(pDX, IDC_ICMP_Code, m_ICMP_Code);
	DDV_MinMaxByte(pDX, m_ICMP_Code, 0, 255);
	DDX_Text(pDX, IDC_ICMP_Type, m_ICMP_Type);
	DDV_MinMaxByte(pDX, m_ICMP_Type, 0, 255);
	DDX_Text(pDX, IDC_ICMP_ID, m_ICMP_ID);
	DDV_MinMaxUInt(pDX, m_ICMP_ID, 0, 65535);
	DDX_Text(pDX, IDC_ICMP_Sequence, m_ICMP_Sequence);
	DDV_MinMaxUInt(pDX, m_ICMP_Sequence, 0, 65535);
	DDX_Text(pDX, IDC_ICMP_Timestamp, m_ICMP_Timestamp);
	DDX_Text(pDX, IDC_EDIT_SourceFile, m_sFilePathName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIPCovertSenderDlg, CDialog)
//{{AFX_MSG_MAP(CIPCovertSenderDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(ID_SEND_IP, OnSendIp)
ON_BN_CLICKED(IDC_OpenFile, OnOpenFile)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCovertSenderDlg message handlers

BOOL CIPCovertSenderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPCovertSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIPCovertSenderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIPCovertSenderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//计算IP包头的校验和
USHORT checksum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	while(size > 1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	if(size) 
	{ 
		cksum += *(UCHAR*)buffer; 
	} 
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum); 
}

//检查输入是否合法
BOOL CIPCovertSenderDlg::CheckValidIP()
{
	BOOL bOK;
	bOK=FALSE;
	
	if (!IPCtrlToSTR(&m_SrcAddress))
		MessageBox("Please enter a valid source address!");
	else if (!IPCtrlToSTR(&m_DestAddress))
		MessageBox("Please enter a valid destination address!");
	else if (m_TypeOfService==-1)
		MessageBox("Please enter a valid type of service!");
	else if (m_FragmentationType==-1)
		MessageBox("Please enter a valid fragmentation size!");
	else
		bOK=TRUE;
	
	return bOK;
}

//将IP地址转换为字符串类型
LPSTR CIPCovertSenderDlg::IPCtrlToSTR(CIPAddressCtrl* ctrl)
{
	BYTE bOctet1;
	BYTE bOctet2;
	BYTE bOctet3;
	BYTE bOctet4;
	
	int iBlank;
	iBlank=ctrl->GetAddress(bOctet1,bOctet2,bOctet3,bOctet4);
	
	if (iBlank!=4)
		return NULL;
	else
	{
		in_addr iAddr;
		iAddr.S_un.S_un_b.s_b1=bOctet1;
		iAddr.S_un.S_un_b.s_b2=bOctet2;
		iAddr.S_un.S_un_b.s_b3=bOctet3;
		iAddr.S_un.S_un_b.s_b4=bOctet4;
		
		return inet_ntoa(iAddr);
	}
}

//发送数据包
void CIPCovertSenderDlg::OnSendIp() 
{
	UpdateData(TRUE);
	
	DWORD szDestIp;
	((CIPAddressCtrl*)GetDlgItem(IDC_DestAddress))->GetAddress(szDestIp);
	DWORD szSourceIp;
	((CIPAddressCtrl*)GetDlgItem(IDC_SrcAddress))->GetAddress(szSourceIp);
	
	char szMsg[] = "";
	int nMsgLen = strlen(szMsg);
	
	//创建原始套接字
	SOCKET sRaw = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	//设置IP首部包含选项
	BOOL bIncl = TRUE;
	::setsockopt(sRaw, IPPROTO_IP, IP_HDRINCL, (char *)&bIncl, sizeof(bIncl));
	
	char buff[1024] = { 0 };
	
	//开始填充IP首部
	IPHeader *pIphdr = (IPHeader *)buff;
	pIphdr->iphVerLen = (m_Version<<4 | m_HeaderSize);
	pIphdr->ipLength = m_TotalLenth;
	pIphdr->ipTTL = m_TTL;
	pIphdr->ipProtocol = m_Protocol;
	pIphdr->ipSource = htonl(szSourceIp);
	pIphdr->ipDestination = htonl(szDestIp);
	pIphdr->ipChecksum = m_CheckSum;
	pIphdr->ipTOS = m_TypeOfService;
	pIphdr->ipFlags = htons((USHORT)(m_FragmentationType<<13 | m_Offset));
	pIphdr->ipID = htons(m_Identification);
	
	//开始填充ICMP首部
	ICMP_HEADER *pIcmphdr = (ICMP_HEADER *)&buff[sizeof(IPHeader)];
	pIcmphdr->i_type = m_ICMP_Type;
	pIcmphdr->i_code = m_ICMP_Code;
	pIcmphdr->i_cksum = htons(m_ICMP_Checksum);
	pIcmphdr->i_id = htons(m_ICMP_ID);
	pIcmphdr->i_seq = htons(m_ICMP_Sequence);
	pIcmphdr->timestamp = htonl(m_ICMP_Timestamp);
	
	char *pData = &buff[sizeof(IPHeader) + sizeof(ICMP_HEADER)];
	memcpy(pData, szMsg, nMsgLen);
	
	// 设置目的地址
	SOCKADDR_IN destAddr = { 0 };
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = 0;
	destAddr.sin_addr.S_un.S_addr = htonl(szDestIp);
	
	// 发送原始ICMP数据包 
	int nRet,j=0;
	CString str;
	for(int i=0; i<num_of_packet; i++,j=j+1)
	{
		pIcmphdr->i_code = filebuff[j];
		pData = &buff[sizeof(IPHeader) + sizeof(ICMP_HEADER)];
		memcpy(pData, szMsg, nMsgLen);
		nRet = ::sendto(sRaw, buff, sizeof(IPHeader) + sizeof(ICMP_HEADER) + nMsgLen, 
			0, (sockaddr*)&destAddr, sizeof(destAddr));
		if(nRet == SOCKET_ERROR)
		{
			AfxMessageBox(" sendto() failed.");
			break;
		}		
	}
	
	str.Format("%d",nRet);
	str="成功发送 "+str+" 字节!";
	AfxMessageBox(str);
	
	::closesocket(sRaw);
}

//打开文件
void CIPCovertSenderDlg::OnOpenFile() 
{
	CFileDialog m_ldFile(TRUE);
	
	// 显示打开文件对话框，并捕捉结果
	if(m_ldFile.DoModal()==IDOK)
	{
		// 捕获所选文件名
		m_sFilePathName=m_ldFile.GetPathName();
		m_sFileName=m_ldFile.GetFileName();
		// 更新对话框
		UpdateData(FALSE); 
		// 按钮可用
		GetDlgItem(ID_SEND_IP)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
	FillData();	
}

//填充读取的数据到缓存
void CIPCovertSenderDlg::FillData()
{
	FILE *fpin;
	CString str;
	int fh,ch,i,filelength;
	
	// 用户输入更新变量
	UpdateData(TRUE);
	
	if((fpin=fopen(m_sFilePathName,"rb"))==NULL)
	{
		str="打开文件 "+m_sFileName+" 失败";
		MessageBox(str);
		exit(0);
	}
	
	fh = _open( m_sFilePathName, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE );
	filelength=_filelength( fh );
	
	if(filelength>1024)
	{
		str="文件 "+m_sFileName+" 长度大于1024!";
		MessageBox(str);
		exit(0);
	}
	
	BeginWaitCursor();
	
	ch=fgetc(fpin);
	i=0;
    while(ch!=-1)
	{				
		filebuff[i++]=ch;
		ch=fgetc(fpin);
	}
	
	fclose(fpin);
	num_of_packet=filelength;
}

