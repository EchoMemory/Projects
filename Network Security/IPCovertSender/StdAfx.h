// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__37C03875_3BAC_46EB_8D0D_7B141AA1A3A3__INCLUDED_)
#define AFX_STDAFX_H__37C03875_3BAC_46EB_8D0D_7B141AA1A3A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "./common/initsock.h"
#include "./common/protoinfo.h"
#include "./common/comm.h"

#include <stdio.h>
#include <ws2tcpip.h>	// 定义了IP_HDRINCL

/*#include "winsock2.h"
#include "mstcpip.h" //用于SIO_RCVALL
#include <ws2tcpip.h> //用于IP_HDRINCL标记

#pragma comment(lib,"ws2_32.lib")*/


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__37C03875_3BAC_46EB_8D0D_7B141AA1A3A3__INCLUDED_)
