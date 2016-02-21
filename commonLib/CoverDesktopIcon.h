#pragma once
#include <tchar.h>
#include "DeskIcon.h"
#include <windows.h>
#include <string>
#include <process.h>
using namespace std;

class CCoverDesktopIcon
{
public:
	CCoverDesktopIcon();
	~CCoverDesktopIcon();
	void SetIconName(LPCTSTR szIconName);
	void SetUrl(LPCTSTR szUrl);
	void Cover();
	BOOL UpdateIconRect();
	BOOL Open(LPCTSTR szFile);
private:
	string m_sIconName;
	string m_sUrl;
	unsigned int m_uThreadId;
	HANDLE m_hThread;
	HWND m_hWnd;
	HDC m_hMemDc;
	HBITMAP m_hBmp;
	unsigned char* m_pBmpData;
	string m_sLnkPath;
	RECT m_rcIcon;
private:
	static unsigned int WINAPI CoverThread(LPVOID lp);
	static LPCTSTR GetWndClass();
	static HRESULT WINAPI CDIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL CreateCDIWindow();
	void DrawWindow();
};

