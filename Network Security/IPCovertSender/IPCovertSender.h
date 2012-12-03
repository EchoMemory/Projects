// IPCovertSender.h : main header file for the IPCOVERTSENDER application
//

#if !defined(AFX_IPCOVERTSENDER_H__ADBD6F42_AF40_4ED6_892D_1AFCB2039D7F__INCLUDED_)
#define AFX_IPCOVERTSENDER_H__ADBD6F42_AF40_4ED6_892D_1AFCB2039D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIPCovertSenderApp:
// See IPCovertSender.cpp for the implementation of this class
//

class CIPCovertSenderApp : public CWinApp
{
public:
	CIPCovertSenderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCovertSenderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIPCovertSenderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCOVERTSENDER_H__ADBD6F42_AF40_4ED6_892D_1AFCB2039D7F__INCLUDED_)
