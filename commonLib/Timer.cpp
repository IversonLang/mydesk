#include "Timer.h"


map<UINT_PTR,TimerParamStruct*> CTimer::ms_mapTimer;


#ifdef _WIN64
void CALLBACK CTimer::KpTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerId, DWORD dwTime)
#else
void CALLBACK CTimer::KpTimerProc(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
#endif
{
    map<UINT_PTR,TimerParamStruct*>::iterator it = ms_mapTimer.find(iTimerId);
    if (it != ms_mapTimer.end()){
        TimerParamStruct* pParam = it->second;
        if (!pParam->bIsLoop){
            ::KillTimer(NULL,iTimerId);
            pParam->fnCall(pParam->pParam);
            delete pParam;
            pParam = NULL;
            ms_mapTimer.erase(it);
        }else{
            pParam->fnCall(pParam->pParam);
        }
    }else{
        ::KillTimer(NULL,iTimerId);
    }
}

UINT_PTR CTimer::SetTimeOut( KPTIMERPROC fn, LPVOID lp, DWORD dwTime, BOOL bIsLoop )
{
    UINT_PTR iTimerId = ::SetTimer(NULL,NULL,dwTime,KpTimerProc);
    TimerParamStruct* pParam = new TimerParamStruct;
    pParam->bIsLoop = bIsLoop;
    pParam->fnCall = fn;
    pParam->pParam = lp;
    pParam->dwTime = dwTime;
    ms_mapTimer.insert(pair<UINT_PTR,TimerParamStruct*>(iTimerId,pParam));
    return iTimerId;
}

UINT_PTR CTimer::SetInterval( KPTIMERPROC fn,LPVOID lp,DWORD dwTime )
{
    return SetTimeOut(fn,lp,dwTime,TRUE);
}

void CTimer::KillTimer( UINT_PTR iTimerId )
{
    ::KillTimer(NULL,iTimerId);
    map<UINT_PTR,TimerParamStruct*>::iterator it = ms_mapTimer.find(iTimerId);
    if (it != ms_mapTimer.end()){
        delete it->second;
        ms_mapTimer.erase(it);
    }
}

BOOL CTimer::GetTimerInfo( UINT_PTR iTimerId,TimerParamStruct& TimerInfo )
{
    map<UINT_PTR,TimerParamStruct*>::iterator it = ms_mapTimer.find(iTimerId);
    if (it != ms_mapTimer.end()){
        memcpy(&TimerInfo,it->second,sizeof(TimerParamStruct));
        return TRUE;
    }
    return FALSE;
}
