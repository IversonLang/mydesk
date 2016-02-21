// cdeskcollator.cpp : 定义 DLL 应用程序的导出函数。
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
	// 程序退出时 g_sqlHelper和g_deskHelper 内存自动释放
}

// vc得到屏幕的当前分辨率方法
// http://www.kuqin.com/shuoit/20131124/336528.html
// 获取可用桌面大小
SIZE getAvailableDeskSize()
{
	int cx = GetSystemMetrics ( SM_CXSCREEN );
	int cy= GetSystemMetrics ( SM_CYSCREEN );
	SIZE deskSize = {cx, cy};

	return deskSize;
}

// 获取整个屏幕分辨率
SIZE getDeskScreenSize()
{
	int  cx = GetSystemMetrics(   SM_CXSCREEN   );   
	int  cy = GetSystemMetrics(   SM_CYSCREEN   );
	SIZE deskSize = {cx, cy};

	return deskSize;
}


void setWndInfo(HWND hWnd)
{
	// 1、获取桌面窗口大小 2、设置窗口大小 3、设置桌面为父窗口
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