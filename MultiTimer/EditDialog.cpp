// EditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MultiTimer.h"
#include "EditDialog.h"
#include "afxdialogex.h"


// CEditDialog dialog

IMPLEMENT_DYNAMIC(CEditDialog, CDialogEx)

CEditDialog::CEditDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDialog::IDD, pParent)
	, m_strEdit(_T(""))
{

}

CEditDialog::~CEditDialog()
{
}

void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
}


BEGIN_MESSAGE_MAP(CEditDialog, CDialogEx)
END_MESSAGE_MAP()


// CEditDialog message handlers
