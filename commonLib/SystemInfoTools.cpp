#include "SystemInfoTools.h"
#include <Windows.h>

namespace CDESK
{
	VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
	{
		if (NULL==lpSystemInfo)    return;
		typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
		LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress( GetModuleHandle(L"kernel32"), "GetNativeSystemInfo");;
		if (NULL != fnGetNativeSystemInfo)
		{
			fnGetNativeSystemInfo(lpSystemInfo);
		}
		else
		{
			GetSystemInfo(lpSystemInfo);
		}
	}

	int SystemInfoTools::isXp()
	{
		int nReValue = 0;

		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if (!GetVersionEx(&osvi)) 
		{
			return -1;
		}

		if (osvi.dwMajorVersion == 5)
		{
			switch (osvi.dwMinorVersion)
			{
			case 0:
				// strcpy(systeminfo,"Windows 2000");
				break;
			case 1:
				// strcpy(systeminfo,"Windows XP");
				nReValue = 1;
				break;
			case 2:
				// strcpy(systeminfo,"Windows Server 2003");
				break;
			default:
				// strcpy(systeminfo,"Unknown");
				break;
			}

		}
		else if (osvi.dwMajorVersion == 6)
		{

			switch (osvi.dwMinorVersion) {
			case 0:
				// strcpy(systeminfo,"Windows Vista");
				break;
			case 1:
				// strcpy(systeminfo,"Windows 7");
				break;
			case 2:
				// strcpy(systeminfo,"Windows 8");
				break;
			default:
				// strcpy(systeminfo,"Unknown");
				break;
			}
		}
		else
		{
			// strcpy(systeminfo,"Unknown");
		}

		return nReValue;
	}

	bool SystemInfoTools::is64()
	{
		SYSTEM_INFO si;
		SafeGetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

