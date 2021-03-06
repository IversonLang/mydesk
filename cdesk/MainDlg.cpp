// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "DeskCollatorFun.h"
#include <dwmapi.h>
#include "wtl.mini/souimisc.h"
#pragma comment(lib,"dwmapi.lib")

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
	, m_hCollatorModule(NULL)
	, m_wallpaperStyle(2)
	, m_wallpaperPaht(L"")
	, m_wallpaperBitmap(NULL)
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
	shellNotifyIcon.Hide();
	if (m_hCollatorModule != NULL)
	{
		FreeLibrary(m_hCollatorModule);
	}
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	// 窗口创建成功后初始化
	initDesk();

	shellNotifyIcon.Create(m_hWnd,GETRESPROVIDER->LoadIcon(_T("ICON_LOGO"),16));
	shellNotifyIcon.Show();

// 	char *p;
// 	memset(p, 0, 200);


	return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	PostMessage(WM_QUIT);
}


void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;

	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;

	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

#include <helper/smenu.h>
LRESULT CMainDlg::OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	switch (lParam)
	{
	case  WM_RBUTTONDOWN:
		{
			//显示右键菜单
			SMenu menu;
			menu.LoadMenu(_T("menu_tray"),_T("smenu"));
			POINT pt;
			GetCursorPos(&pt);
			menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
		}break;
	case WM_LBUTTONDOWN:
		{
			//显示/隐藏主界面
			if (IsIconic())
				ShowWindow(SW_SHOWNORMAL);
			else
				ShowWindow(SW_MINIMIZE);
		}break;
	default:
		break;
	}
	return S_OK;
}

//演示如何响应菜单事件
void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uNotifyCode == 0)
	{
		switch (nID)
		{
		case 6:
			PostMessage(WM_CLOSE);
			break;
		default:
			break;
		}
	}
}

void CMainDlg::initDesk()
{
	
	m_hCollatorModule = LoadLibrary(L"cdeskcollator.dll");

	if (m_hCollatorModule != NULL)
	{
		HWND deskWnd = this->m_hWnd;
		setWndInfoFun setWndInfo = NULL;
		setWndInfo = (setWndInfoFun)GetProcAddress(m_hCollatorModule, "setWndInfo");
		setWndInfo(deskWnd);
		updateDeskBackground();
	}
	else
	{
		//@exception
	}
}

void CMainDlg::updateDeskBackground()
{
	if (m_hCollatorModule != NULL)
	{
		updateDeskBackgroundFun updateBk = NULL;
		updateBk = (updateDeskBackgroundFun)GetProcAddress(m_hCollatorModule, "updateDeskBackground");
		updateBk(m_wallpaperPaht, m_wallpaperStyle);

		CRect rcList = GetWindowRect();
		m_wallpaperBitmap = SResLoadFromFile::LoadImage(m_wallpaperPaht.c_str());
		if (2 == m_wallpaperStyle)
		{
			int nImageWidth = m_wallpaperBitmap->Width();
			int nImageHeight = m_wallpaperBitmap->Height();
			int nDesktopWidth = rcList.Width();
			int nDesktopHeight = rcList.Height();

			if ((nImageWidth / nImageHeight) == (nDesktopWidth / nDesktopHeight))
			{
				m_srcBitmapRect = CRect(0, 0, nImageWidth, nImageHeight);
			}
			else if ((nImageWidth / nImageHeight) > (nDesktopWidth / nDesktopHeight))
			{				
				int nWith = (nDesktopWidth / nDesktopHeight) * nImageHeight;
				m_srcBitmapRect = CRect((nImageWidth-nWith)/2, 0, nWith, nImageHeight);
			}
			else if ((nImageWidth / nImageHeight) < (nDesktopWidth / nDesktopHeight))
			{
				int nHeight = (nDesktopHeight / nDesktopWidth) * nImageWidth;
				m_srcBitmapRect = CRect(0, (nImageHeight-nHeight)/2, nImageWidth, nHeight);
			}
		}				
	}
}

void CMainDlg::OnPaint(IRenderTarget * pRT)
{
	// 自绘背景
	SPainter painter;
	BeforePaint(pRT, painter);
	CRect rcList = GetWindowRect();
	pRT->PushClipRect(&rcList);

	if (m_wallpaperBitmap != NULL)
	{
		pRT->DrawBitmapEx(rcList, m_wallpaperBitmap, &m_srcBitmapRect, EM_STRETCH);
	}
	else
	{

	}

	pRT->PopClip();
	AfterPaint(pRT, painter);
}

