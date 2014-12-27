
// MultiTimer.h : main header file for the MultiTimer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMultiTimerApp:
// See MultiTimer.cpp for the implementation of this class
//

class CMultiTimerApp : public CWinApp
{
public:
	CMultiTimerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMultiTimerApp theApp;
