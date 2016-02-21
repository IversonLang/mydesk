#ifndef __KPBASELIB_SOFTINSTALLCHECK__
#define __KPBASELIB_SOFTINSTALLCHECK__

#include "registry.h"
#include <TlHelp32.h>
#include <shlobj.h>
#pragma comment( lib, "shell32.lib")
#include <string>
#include <vector>
using namespace std;

namespace Soft{
	struct SoftInstallCheckStruct{
		char szSoftName[MAX_PATH];
		char szSoftDeskIconName[MAX_PATH];
		char szSoftProcessName[MAX_PATH];
		char szSoftRegCheckValue[MAX_PATH];
		SoftInstallCheckStruct(){
			memset(szSoftName,0,sizeof(szSoftName));
			memset(szSoftDeskIconName,0,sizeof(szSoftDeskIconName));
			memset(szSoftProcessName,0,sizeof(szSoftProcessName));
			memset(szSoftRegCheckValue,0,sizeof(szSoftRegCheckValue));
		}
		SoftInstallCheckStruct(const char* pSoftName,const char* pSoftProcessName,const char* pSoftDeskIconName,const char* pSoftRegCheckValue){
			if (pSoftName){
				strcpy(szSoftName,pSoftName);
			}
			if (pSoftDeskIconName){
				strcpy(szSoftDeskIconName,pSoftDeskIconName);
			}
			if (pSoftProcessName){
				strcpy(szSoftProcessName,pSoftProcessName);
			}
			if(pSoftRegCheckValue){
				strcpy(szSoftRegCheckValue,pSoftRegCheckValue);
			}
		}
	};

	DWORD GetProcessIdByName(LPCTSTR lpFilename);
	BOOL GetLocalPath(int csidl,string* pValue);
	BOOL CheckSoftFromReg(const char* szDisplayName);
	BOOL CheckSoftFromProcessName(const char* szProcessName);
	BOOL CheckSoftFromDeskIconName(const char* szDeskIconName);
	BOOL CheckSoftFromRegValue(const char* szSoftRegCheckValue);
	BOOL IsSoftInstall(SoftInstallCheckStruct* pSic);

	//判断是不是网吧
	BOOL IsCybercafe(vector<string>& vecReg,vector<string>& vecIcon,vector<string>& vecProcess);

	BOOL AdjustPurview();
	BOOL GetProcessInfoById(DWORD dwProcessId,PROCESSENTRY32* pInfo);
	BOOL GetCurAndParentModuleFile(string& sCurModule,string& sParentModule);

	//分割字符串
	BOOL SplitString(LPCSTR pszStr,LPCSTR pszMark,vector<string>& vecOut);
	void ReplaceAll(string& sStr,const char* src,const char* dst);
	//检测图标是否存在，并返回文件路径
	BOOL IsIconExist(LPCSTR szName,string& sIconPath);
	//字符串转大写 处理中文字符情况
	string ToUpper(const char* pStr);
	char* ToUpper(char* pStr);
}

#endif //(__KPBASELIB_SOFTINSTALLCHECK__)