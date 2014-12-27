
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window
#include <map>
#include <string>
class stDescription
{
public:
	stDescription(std::string desc, int period, bool a = true):desc(desc),period(period),alertable(a){}
	std::string desc;
	int period;
	bool alertable;
	void Alert();
};

typedef std::multimap<int, stDescription> TIMER;
typedef TIMER::iterator TIMER_ITER;
typedef TIMER::reverse_iterator TIMER_ITER_R;
#define FOREACHTIMER(it)	for(TIMER_ITER it=m_mapTimer.begin();it!=m_mapTimer.end();it++)
#define FOREACHTIMER_R(it)	for(TIMER_ITER_R it=m_mapTimer.rbegin();it!=m_mapTimer.rend();it++)


class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
	TIMER_ITER Select();
	void New(int ts, std::string desc, int period);
	bool Del();
	bool Save(std::string filename);
	bool Load(std::string filename);
private:
	TIMER m_mapTimer;
	int m_nSelect;
	int m_nSubSel;
	int m_nOffset;

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

