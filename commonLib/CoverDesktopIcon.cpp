#include "CoverDesktopIcon.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include <shtypes.h>
#include <shlobj.h>

enum{
	UM_BEGIN = WM_USER + 244,
	UM_FIX,

	TIMER_BEGIN = WM_USER + 2244,
	TIMER_FOR_PAINT
};

CCoverDesktopIcon::CCoverDesktopIcon()
{
	m_hWnd = nullptr;
	m_hMemDc = CreateCompatibleDC(nullptr);
	m_hBmp = nullptr;
	m_uThreadId = 0;
	m_hThread = nullptr;
}

CCoverDesktopIcon::~CCoverDesktopIcon()
{
	TerminateThread(m_hThread, 0);
	DeleteDC(m_hMemDc);
	if (m_hBmp){
		DeleteObject(m_hBmp);
	}
}

void CCoverDesktopIcon::Cover()
{
	if (m_uThreadId == 0){
		m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CoverThread, this, 0, &m_uThreadId);
	}
}

unsigned int WINAPI CCoverDesktopIcon::CoverThread(LPVOID lp)
{
	CCoverDesktopIcon* _this = (CCoverDesktopIcon*)lp;
	_this->CreateCDIWindow();
	return 0;
}

LPCTSTR CCoverDesktopIcon::GetWndClass()
{
	return "_BicWndClass_";
}

BOOL CCoverDesktopIcon::CreateCDIWindow()
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//注册窗口类
	WNDCLASSEX wcex = { 0 };
	if (!GetClassInfoEx(hInstance, GetWndClass(), &wcex)) {
		wcex.cbSize = sizeof(WNDCLASSEX);                   //WNDCLASSEX结构体大小
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; //位置改变时重绘
		wcex.lpfnWndProc = CDIWndProc;                           //消息处理函数
		wcex.hInstance = hInstance;                    //当前实例句柄
		wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;            //背景色
		wcex.lpszClassName = GetWndClass();							//参窗口类名
		wcex.hIcon = NULL;                                 //图标
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);          //光标
		wcex.lpszMenuName = NULL;                                 //菜单名称
		wcex.hIconSm = NULL;                                 //最小化图标
		RegisterClassEx(&wcex);
	}

	m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, GetWndClass(), m_sIconName.c_str(), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 64, 64,
		CDeskIcon::GetDeskTopWindow(), nullptr, hInstance, this);

	DWORD D = GetLastError();



	SendMessage(m_hWnd, UM_FIX, 0, 0);
	DrawWindow();
	SetTimer(m_hWnd, TIMER_FOR_PAINT, 5000, nullptr);
	SetParent(m_hWnd, CDeskIcon::GetDeskTopWindow());


	string sTipsText = "位置: ";
	if (m_hWnd && !m_sLnkPath.empty()){
		DragAcceptFiles(m_hWnd, TRUE);
		sTipsText += m_sLnkPath;
	}
	else{
		sTipsText += "Internet";
	}

	//设置提示窗口的信息 
	RECT rcClient = { 0, 0, 256, 256 };
	TOOLINFO tti = { 0 };
	tti.cbSize = sizeof(TOOLINFO);
	tti.uFlags = TTF_SUBCLASS;
	tti.hwnd = m_hWnd;
	tti.rect = rcClient;
	tti.hinst = hInstance;
	tti.lpszText = (LPSTR)sTipsText.c_str();

	HWND hwndTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL, hInstance, NULL);
	//新增一个提示  
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&tti);

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

HRESULT WINAPI CCoverDesktopIcon::CDIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CCoverDesktopIcon* _this = (CCoverDesktopIcon*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST && uMsg != WM_LBUTTONDBLCLK){
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		ClientToScreen(hWnd, &pt);
		PostMessage(CDeskIcon::GetDeskTopWindow(), uMsg, wParam, MAKELPARAM(pt.x, pt.y));
	}

	if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST){
		PostMessage(CDeskIcon::GetDeskTopWindow(), uMsg, wParam, lParam);
	}

	switch (uMsg)
	{

	case WM_CREATE:{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pcs->lpCreateParams);
	}break;

	case WM_MOUSEMOVE:{

		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = hWnd;// 指定要 追踪 的窗口 
		csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
		::_TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持

	}break;

	case WM_DROPFILES:{
		HDROP hDrop = (HDROP)wParam;
		char * szFileName = new char[255];
		int nFileNum = DragQueryFile(hDrop, -1, NULL, 0);//得到拖动文件个数
		for (int i = 0; i < nFileNum; i++){
			DragQueryFile(hDrop, i, szFileName, 255); //szFileName第i个文件名
			_this->Open(szFileName);
		}
	}break;

	case WM_LBUTTONDBLCLK:{
		_this->Open(_this->m_sUrl.c_str());
	}break;


	case WM_TIMER:{
		if (wParam == TIMER_FOR_PAINT){
			_this->UpdateIconRect();
			_this->DrawWindow();
		}
	}break;

	case UM_FIX:{
		if (_this->UpdateIconRect()){
			ShowWindow(hWnd, SW_SHOW);
		}
		else{
			ShowWindow(hWnd, SW_HIDE);
		}

	}break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CCoverDesktopIcon::SetIconName(LPCTSTR szIconName)
{
	m_sIconName = szIconName;
	string sLinkFile;
	CDeskIcon::GetIconPath(szIconName, sLinkFile);
	CDeskIcon::GetLnkFilePath(sLinkFile.c_str(), m_sLnkPath);
	if (m_hWnd && !m_sLnkPath.empty()){
		DragAcceptFiles(m_hWnd, TRUE);
	}
	UpdateIconRect();
}

void CCoverDesktopIcon::SetUrl(LPCTSTR szUrl)
{
	m_sUrl = szUrl;
}

BOOL CCoverDesktopIcon::UpdateIconRect()
{
	RECT rc = { 0 };
	BOOL b = CDeskIcon::GetDeskTopIconRect(m_sIconName.c_str(), rc);

	//判断图标大小是否改变
	if (rc.right - rc.left != m_rcIcon.right - m_rcIcon.left
		|| rc.bottom - rc.top != m_rcIcon.bottom - m_rcIcon.top){
		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biHeight = -abs(rc.bottom - rc.top);
		bmi.bmiHeader.biWidth = abs(rc.right - rc.left);
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		m_hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&m_pBmpData, NULL, NULL);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(m_hMemDc, m_hBmp);
		memset(m_pBmpData, 2, abs(rc.bottom - rc.top)*abs(rc.right - rc.left) * 4);
		DeleteObject(hOldBmp);

		//SetWindowPos(m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
	}
	memcpy(&m_rcIcon, &rc, sizeof(RECT));
	return b;
}

void CCoverDesktopIcon::DrawWindow()
{
	HDC hPaintDc = GetDC(m_hWnd);

	POINT ptSrc = { 0 };
	POINT ptDst = { m_rcIcon.left, m_rcIcon.top };
	SIZE siDst = { m_rcIcon.right - m_rcIcon.left, m_rcIcon.bottom - m_rcIcon.top };
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;
	typedef BOOL(WINAPI *FnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT* pptDst, SIZE* psize, HDC hdcSrc, POINT* pptSrc, COLORREF crKey, BLENDFUNCTION* pblend, DWORD dwFlags);
	static FnUpdateLayeredWindow pUpdateLayeredWindow = NULL;
	if (pUpdateLayeredWindow == NULL){
		HMODULE hDll = LoadLibrary("User32.DLL");
		pUpdateLayeredWindow = (FnUpdateLayeredWindow)GetProcAddress(hDll, "UpdateLayeredWindow");
	}
	if (pUpdateLayeredWindow) {
		pUpdateLayeredWindow(m_hWnd, hPaintDc, &ptDst, &siDst, m_hMemDc, &ptSrc, 0, &bf, ULW_ALPHA);
	}

	ReleaseDC(m_hWnd, hPaintDc);
}

BOOL CCoverDesktopIcon::Open(LPCTSTR szFile)
{
	if (!m_sLnkPath.empty()){
		ShellExecute(nullptr, "open", m_sLnkPath.c_str(), szFile, nullptr, SW_SHOW);
		return TRUE;
	}
	else{
		ShellExecute(nullptr, "open", szFile, nullptr, nullptr, SW_SHOW);
	}
	return FALSE;
}
