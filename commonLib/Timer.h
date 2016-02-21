#pragma once
#pragma warning(disable:4786)
#include <Windows.h>
#include <map>
using namespace std;

typedef void (*KPTIMERPROC)(LPVOID);
struct TimerParamStruct{
    KPTIMERPROC fnCall;
    LPVOID pParam;
    DWORD dwTime;
    BOOL bIsLoop;
};
class CTimer
{
protected:
    CTimer(void){};
    ~CTimer(void){};
public:
    static UINT_PTR SetTimeOut(KPTIMERPROC fn, LPVOID lp, DWORD dwTime, BOOL bIsLoop = FALSE);
    static UINT_PTR SetInterval(KPTIMERPROC fn, LPVOID lp, DWORD dwTime);
    static void KillTimer(UINT_PTR iTimerId);
#ifdef _WIN64
	static void CALLBACK KpTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerId, DWORD dwTime);
#else
	static void CALLBACK KpTimerProc(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime);
#endif
    static BOOL GetTimerInfo(UINT_PTR iTimerId,TimerParamStruct& TimerInfo);
private:
    static map<UINT_PTR,TimerParamStruct*> ms_mapTimer;
};

