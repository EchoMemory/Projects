// IPCovertSenderDlg.h : header file
//

#if !defined(AFX_IPCOVERTSENDERDLG_H__B51779F8_EA9A_4DAE_8991_512F7D8A3123__INCLUDED_)
#define AFX_IPCOVERTSENDERDLG_H__B51779F8_EA9A_4DAE_8991_512F7D8A3123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIPCovertSenderDlg dialog

class CIPCovertSenderDlg : public CDialog
{
// Construction
public:
	void FillData();
	CIPCovertSenderDlg(CWnd* pParent = NULL);	// standard constructor

	CString m_sFileName;

// Dialog Data
	//{{AFX_DATA(CIPCovertSenderDlg)USHORT
	enum { IDD = IDD_IPCOVERTSENDER_DIALOG };
	CIPAddressCtrl	m_SrcAddress;
	CIPAddressCtrl	m_DestAddress;
	UINT	m_CheckSum;
	UINT	m_FragmentationType;
	BYTE	m_HeaderSize;
	UINT	m_Identification;
	UINT	m_Offset;
	BYTE	m_Protocol;
	UINT	m_TotalLenth;
	BYTE	m_TypeOfService;
	BYTE	m_Version;
	BYTE	m_TTL;
	UINT	m_ICMP_Checksum;
	BYTE	m_ICMP_Code;
	BYTE	m_ICMP_Type;
	UINT	m_ICMP_ID;
	UINT	m_ICMP_Sequence;
	UINT	m_ICMP_Timestamp;
	CString	m_sFilePathName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCovertSenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIPCovertSenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSendIp();
	afx_msg void OnOpenFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	//Check that all the IP parameters are valid
	BOOL CheckValidIP();

	//Check we have a valid address
	LPSTR IPCtrlToSTR(CIPAddressCtrl* ctrl);

	/*SOCKET m_RawSock; //SOCK
	int err;
	int CreateSock(); //…Ë÷√SOCK
	BOOL InitSock(); //º”‘ÿSOCK.DLL*/
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCOVERTSENDERDLG_H__B51779F8_EA9A_4DAE_8991_512F7D8A3123__INCLUDED_)
