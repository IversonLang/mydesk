#pragma once

#ifndef _WINDOWS_
	#include <windows.h>
#endif

namespace CrashRP
{
	#ifdef CRASHRP_EXPORTS
		#define CrashAPI __declspec(dllexport)
	#else
		#define CrashAPI __declspec(dllimport)

	#   ifdef _DEBUG
	#       pragma comment(lib, "CrashReport_Debug.lib")
	#   else
	#       pragma comment(lib, "CrashReport.lib")
	#   endif
#endif

	extern "C"
	{
		// ����
		CrashAPI bool Start();

		// ֹͣ
		CrashAPI bool Stop();

		// ���÷�������Ϣ
		CrashAPI bool SetServInfo(TCHAR *pServAddr, DWORD uServPort);
	}
}