
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "MultiTimer.h"
#include "ChildView.h"

#include <fstream>
#include "TimerDialog.h"
#include "EditDialog.h"
using namespace std;
enum
{
	TIMERID_REFRESHPERSECOND = 1,
};
#define PAGEITEM 30
#define X 10
#define Y 30
#define W 1024
#define H 20
#define DFLT_FILENAME "default.sav"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void stDescription::Alert()
{
	if(alertable)
	{
		alertable = false;
		MessageBoxA(0,desc.c_str(),"Time's Up", MB_OK|MB_TOPMOST);
	}
}

// CChildView

CChildView::CChildView()
	: m_nSelect(-1)
	, m_nSubSel(-1)
	, m_nOffset(0)
{
	Load(DFLT_FILENAME);
}

CChildView::~CChildView()
{
}
TIMER_ITER CChildView::Select()
{
	TIMER_ITER ret = m_mapTimer.begin();
	int i = 0;
	int sz = (int)m_mapTimer.size();
	while (i < sz)
	{
		if (i == m_nSelect)
		{
			return ret;
		}
		ret++;
		i++;
	}
	return ret;
}
void CChildView::New(int ts, std::string desc, int period)
{
	bool a = (ts - time(0) > 0);
	m_mapTimer.insert(pair<int, stDescription>(ts, stDescription(desc, period, a)));
}
bool CChildView::Del()
{
	TIMER_ITER it = Select();
	if ( it == m_mapTimer.end()) return false;
	m_mapTimer.erase(it);
	return true;
}
bool CChildView::Save(string filename)
{
	ofstream output;
	output.open(filename);
	if (!output.is_open()) return false;
	FOREACHTIMER(it)
	{
		output << it->first << " " << it->second.period << " " << it->second.desc << endl;
	}
	output.close();
	return true;
}
bool CChildView::Load(string filename)
{
	m_mapTimer.clear();

	ifstream input;
	input.open(filename);
	if (!input.is_open()) return false;
	while (!input.eof())
	{
		int ts = -1;
		int period = 0;
		char desc[512];
		input >> ts;
		if (ts == -1) break;
		input >> period;
		input >> desc[0];	//skip \t and space
		input.getline(desc+1, sizeof(desc)-1);
		New(ts, desc, period);
	}
	input.close();
	return true;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	static CFont font;
	static bool init = false;
	if (!init)
	{
		init = true;
		font.CreateFont(
		   16,                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_NORMAL,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   _T("Courier New"));
	}
	dc.SelectObject(font);

	// TODO: Add your message handler code here
	int now = (int)time(0);
	int x = X;
	int y = Y;
	int w = W;
	int h = H;
	dc.TextOutA(x, y-h, "second description");
	int i = 0;
	FOREACHTIMER(it)
	{
		if (i < m_nOffset){ i++; continue; }

		CString msg;
		msg.Format("%6d %s     ", max(it->first - now, 0), it->second.desc.c_str()); 
		if (i == m_nSelect)
		{
			dc.SetBkColor(RGB(200,255,255));
			int start = msg.Find(" ", 0);
			int idx = 0;
			while (1)
			{
				int end = msg.Find(";", start);
				if (m_nSubSel == idx)
				{
					if (end < 0) end = msg.GetLength();
					dc.SetBkColor(RGB(200,255,255));
					dc.TextOut(x, y, msg, start);
					dc.SetBkColor(RGB(255,168,255));
					dc.TextOut(x + start * 8, y, msg.Mid(start, end - start), end - start);
					dc.SetBkColor(RGB(200,255,255));
					dc.TextOut(x + end * 8, y, msg.Mid(end, msg.GetLength() - end), msg.GetLength() - end);
					dc.SetBkColor(RGB(255,255,255));
					break;
				}
				if (end < 0)
				{
					//not found
					dc.SetBkColor(RGB(200,255,255));
					dc.TextOut(x, y, msg);
					dc.SetBkColor(RGB(255,255,255));
					break;
				}
				start = end + 1;
				idx ++;
			}
		}
		else
			dc.TextOutA(x, y, msg);
		y += h;
		i++;
	}

	// Do not call CWnd::OnPaint() for painting messages
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (point.y >= Y && point.y < Y + (int)m_mapTimer.size() * H)
	{
		m_nSelect = (point.y - Y)/H + m_nOffset;
		Invalidate(0);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

const char* help_msg = \
	"(A) Add\n" \
	"(C) Copy\n" \
	"(D) Delete\n" \
	"(E) Empty\n" \
	"(F) Find\n" \
	"(G) Go! (set next period)\n" \
	"(H) Help\n" \
	"(L) Load\n" \
	"(M) Modify\n" \
	"(N) New\n" \
	"(P) Set period\n" \
	"(Q) Quit without save\n" \
	"(R) Reset\n" \
	"(S) Save\n" \
	"(W) Wait\n" \
	"(0) Select first sub item\n" \
	"Arrow Left: Select prior sub item\n" \
	"Arrow Right: Select next sub item\n" \
	"Arrow Up: Select prior item\n" \
	"Arrow Down: Select next item\n" \
	"Home: Select first item\n" \
	"End: Select last item\n" \
	;
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'a': case 'A': case 'n': case 'N':
		{
			CTimerDialog dlg;
			if (dlg.DoModal() == IDOK)
			{
				New(dlg.Time() + (int)time(0), dlg.m_strDescription.GetBuffer(), 0);
				Invalidate();
			}
		}
		break;
	case 'c': case 'C':
		{
			TIMER_ITER it = Select();
			if (it == m_mapTimer.end()) break;
			string desc = it->second.desc;
			int period = it->second.period;
			int ts = it->first;
			New(ts, desc, period);
			Invalidate();
		}
		break;
	case 'd': case 'D':
		if (Del())
		{
			Invalidate();
		}
		break;
	case 'e': case 'E':
		if (MessageBoxA("Sure?", "Warning!", MB_OKCANCEL) == IDOK)
		{
			m_mapTimer.clear();
			Invalidate();
		}
		break;
	case 'f': case 'F':
		{
			CEditDialog dlg;
			if (dlg.DoModal() == IDOK)
			{
				int idx = 0;
				bool found = false;
				FOREACHTIMER(it)
				{
					if (idx <= m_nSelect)
					{
						idx++;
						continue;
					}
					int x = it->second.desc.find(dlg.m_strEdit.GetBuffer());
					if (x >= 0)
					{
						m_nSelect = idx;
						found = true;
						Invalidate(0);
						break;
					}
					idx++;
				}
				if (found) break;
				
				idx = 0;
				FOREACHTIMER(it)
				{
					if (idx > m_nSelect) break;

					int x = it->second.desc.find(dlg.m_strEdit.GetBuffer());
					if (x >= 0)
					{
						m_nSelect = idx;
						found = true;
						Invalidate(0);
						break;
					}
					idx++;
				}
			}
		}
		break;
	case 'g': case 'G':
		{
			TIMER_ITER it = Select();
			if (it == m_mapTimer.end()) break;
			int period = it->second.period;
			string desc = it->second.desc;
			Del();
			New((int)time(0) + period, desc, period);
			Invalidate();
		}
		break;
	case 'h': case 'H':
		MessageBox(help_msg, "Help", MB_OK);
		break;
	case 'l': case 'L':
		if (Load(DFLT_FILENAME))
		{
			Invalidate();
		}
		break;
	case 'm': case 'M':
		{
			TIMER_ITER it = Select();
			if (it == m_mapTimer.end()) break;
			int period = it->second.period;
			CTimerDialog dlg;
			int ts = max(it->first - (int)time(0), 0);
			if (ts == 0)
			{
				ts = period;
			}
			dlg.m_strDescription = it->second.desc.c_str();
			dlg.m_nHour = ts / 3600;
			dlg.m_nMinute = ts / 60 % 60;
			dlg.m_nSecond = ts % 60;
			if (dlg.DoModal() == IDOK)
			{
				Del();
				New(dlg.Time() + (int)time(0), dlg.m_strDescription.GetBuffer(), period);
				Invalidate();
			}
			break;
		}
	case 'p': case 'P':
		{
			TIMER_ITER it = Select();
			if (it == m_mapTimer.end()) break;
			int period = it->second.period;
			CTimerDialog dlg;
			dlg.m_strDescription = it->second.desc.c_str();
			dlg.m_nHour = period / 3600;
			dlg.m_nMinute = period / 60 % 60;
			dlg.m_nSecond = period % 60;
			if (dlg.DoModal() == IDOK)
			{
				it->second.period = dlg.Time();
			}
		}
		break;
	case 'q': case 'Q':
		//Save(DFLT_FILENAME);
		if (MessageBoxA("Are you sure to exit without save?", "Warning", MB_OKCANCEL) == IDOK)
			exit(0);
		break;
	case 'r': case 'R':
		{
			Save(DFLT_FILENAME);	//backup first in case screw up
			CEditDialog dlg;
			if (dlg.DoModal() == IDOK)
			{
				if (dlg.m_strEdit == "") break;

				int odr = 0;
				int shift = 300;	//5 mins
				//Note: support max 300 events only!
				int now = (int)time(0);
				bool found = true;
				while (found)
				{
					found = false;
					FOREACHTIMER(it)
					{
						if (it->first <= now) continue;
						int x = it->second.desc.find(dlg.m_strEdit.GetBuffer());
						if (x >= 0)
						{
							string dest = it->second.desc;
							int period = it->second.period;
							m_mapTimer.erase(it);
							New(now - shift + (odr++), dest, period);
							found = true;
							break;
						}
					}
				}
				Invalidate();
			}
		}
		break;
	case 's': case 'S':
		Save(DFLT_FILENAME);
		break;
	case 'w': case 'W':
		{
			if (m_mapTimer.empty()) break;
			if (m_mapTimer.begin()->first > time(0)) break;

			CTimerDialog dlg;
			dlg.m_strDescription = "Wait All Timers...";
			if (dlg.DoModal() == IDOK)
			{
				int now = (int)time(0);
				int delay = dlg.Time();
				while (1)
				{
					TIMER_ITER it = m_mapTimer.begin();
					if (it == m_mapTimer.end()) break;
					if (it->first <= now)
					{
						New(now + delay, it->second.desc, it->second.period);
						m_mapTimer.erase(it);
					}
					else break;
				}
				Invalidate();
			}
		}
		break;
	case '0':
		m_nSubSel = 0;
		Invalidate(0);
		break;
	case VK_UP:
		m_nSelect--;
		if (m_nSelect < m_nOffset)
		{
			m_nOffset -= PAGEITEM;
			if (m_nOffset < 0) m_nOffset = 0;
		}
		if (m_nSelect < 0)
		{
			m_nSelect = m_mapTimer.size() - 1;
			m_nOffset = m_nSelect - PAGEITEM;
			if (m_nOffset < 0) m_nOffset = 0;
		}
		Invalidate();
		break;
	case VK_DOWN:
		m_nSelect++;
		if (m_nSelect >= m_nOffset + PAGEITEM)
		{
			m_nOffset += PAGEITEM;
		}
		if (m_nSelect >= (int)m_mapTimer.size())
		{
			m_nSelect = 0;
			m_nOffset = 0;
		}
		Invalidate();
		break;
	case VK_LEFT:
		m_nSubSel--;
		Invalidate(0);
		break;
	case VK_RIGHT:
		m_nSubSel++;
		Invalidate(0);
		break;
	case VK_HOME:
		m_nSelect = 0;
		m_nOffset = 0;
		Invalidate();
		break;
	case VK_END:
		m_nSelect = (int)m_mapTimer.size() - 1;
		m_nOffset = m_nSelect - PAGEITEM;
		if (m_nOffset < 0) m_nOffset = 0;
		Invalidate();
		break;
	case VK_PRIOR:
		m_nSelect -= PAGEITEM;
		m_nOffset -= PAGEITEM;
		if (m_nSelect < 0)
		{
			m_nSelect = m_nOffset = 0;
		}
		Invalidate();
		break;
	case VK_NEXT:
		m_nSelect += PAGEITEM;
		m_nOffset += PAGEITEM;
		if (m_nSelect >= (int)m_mapTimer.size())
		{
			m_nSelect = (int)m_mapTimer.size() - 1;
			m_nOffset = m_nSelect - PAGEITEM;
			if (m_nOffset < 0) m_nOffset = 0;
		}
		Invalidate();
		break;
	default: break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMERID_REFRESHPERSECOND:
		{
			int now = (int)time(0);
			FOREACHTIMER(it)
			{
				if (it->first > now) break;
				if (it->second.alertable)
				{
					KillTimer(TIMERID_REFRESHPERSECOND);
					it->second.Alert();
					//disable alert 
					now = (int)time(0);
					FOREACHTIMER(ij)
					{
						if (ij->first > now) break;
						//else
						ij->second.alertable = false;
					}
					SetTimer(TIMERID_REFRESHPERSECOND, 1000, NULL);
					break;
				}
			}
		}
		Invalidate(0);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  Add your specialized creation code here
	SetTimer(TIMERID_REFRESHPERSECOND, 1000, NULL);
	return 0;
}


void CChildView::OnDestroy()
{
	Save(DFLT_FILENAME);
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}
