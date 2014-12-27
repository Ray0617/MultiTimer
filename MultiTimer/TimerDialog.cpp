// TimerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MultiTimer.h"
#include "TimerDialog.h"
#include "afxdialogex.h"


// CTimerDialog dialog

IMPLEMENT_DYNAMIC(CTimerDialog, CDialogEx)

CTimerDialog::CTimerDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimerDialog::IDD, pParent)
	, m_nHour(0)
	, m_nMinute(0)
	, m_nSecond(0)
	, m_strDescription(_T(""))
{

}

CTimerDialog::~CTimerDialog()
{
}

void CTimerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHour);
	DDX_Text(pDX, IDC_EDIT2, m_nMinute);
	DDX_Text(pDX, IDC_EDIT3, m_nSecond);
	DDX_Text(pDX, IDC_EDIT4, m_strDescription);
}


BEGIN_MESSAGE_MAP(CTimerDialog, CDialogEx)
END_MESSAGE_MAP()


// CTimerDialog message handlers
