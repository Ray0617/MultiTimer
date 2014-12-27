#pragma once


// CTimerDialog dialog

class CTimerDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTimerDialog)

public:
	CTimerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTimerDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Time(){return 3600*m_nHour+60*m_nMinute+m_nSecond;}
	int m_nHour;
	int m_nMinute;
	int m_nSecond;
	CString m_strDescription;
};
