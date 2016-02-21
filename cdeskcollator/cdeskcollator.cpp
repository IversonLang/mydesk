// cdeskcollator.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "cdeskcollator.h"
// #include "DeskHelper.h"
// #include "SqliteHelper.h"

#pragma comment(lib,"../lib/CommonLib.lib")


// SqliteHelper *g_sqlHelper = NULL;
// CDeskHelper *g_deskHelper = NULL;

void init()
{
// 	new SSingleton<SqliteHelper>;
// 	g_sqlHelper = SSingleton<SqliteHelper>::getSingletonPtr();
// 	new SSingleton<CDeskHelper>;
// 	g_deskHelper = SSingleton<CDeskHelper>::getSingletonPtr();
}

void uninit()
{
	// �����˳�ʱ g_sqlHelper��g_deskHelper �ڴ��Զ��ͷ�
}

// vc�õ���Ļ�ĵ�ǰ�ֱ��ʷ���
// http://www.kuqin.com/shuoit/20131124/336528.html
// ��ȡ���������С
SIZE getAvailableDeskSize()
{
	int cx = GetSystemMetrics ( SM_CXSCREEN );
	int cy= GetSystemMetrics ( SM_CYSCREEN );
	SIZE deskSize = {cx, cy};

	return deskSize;
}

// ��ȡ������Ļ�ֱ���
SIZE getDeskScreenSize()
{
	int  cx = GetSystemMetrics(   SM_CXSCREEN   );   
	int  cy = GetSystemMetrics(   SM_CYSCREEN   );
	SIZE deskSize = {cx, cy};

	return deskSize;
}


void setWndInfo(HWND hWnd)
{
	// 1����ȡ���洰�ڴ�С 2�����ô��ڴ�С 3����������Ϊ������
	if (NULL == hWnd)
	{
		return;
	}

	SIZE deskSize = getDeskScreenSize();
	::MoveWindow(hWnd, 0, 0, deskSize.cx, deskSize.cy, TRUE);
}


bool updateDeskBackground(std::wstring &wallpaperPath, int &nStyle)
{
	CRegistry reg(HKEY_CURRENT_USER);
	std::string strValue = "";
	int nWallpaperStyle = 0;
	BOOL bRe = reg.Open(L"Control Panel\\Desktop");
	if (bRe) 
	{
		reg.Read(L"Wallpaper", &strValue);

		reg.Read(L"WallpaperStyle", &nWallpaperStyle);
	}

	wallpaperPath = L"c:\\windows\\web\\wallpaper\\theme1\\img3.jpg";
	nStyle = 2;
	
	return true;
}

extern "C" _declspec(dllexport) void __cdecl rundll32dllfun(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine)
{
	MessageBoxA(NULL,"TEST",lpszCmdLine,MB_OK);
	return;
}