#include "../Include/CrashReport/Export.h"
#include "../ReportHandle.h"

namespace CrashRP
{
	// ����
	bool Start()
	{
		return CReportHandle::Instance()->Open();
	}

	// ֹͣ
	bool Stop()
	{
		return CReportHandle::Instance()->Close();
	}

	// ���÷�������Ϣ
	bool SetServInfo(TCHAR *pServAddr, DWORD uServPort)
	{
		return false;
	}
}