#include "StdAfx.h"
#include "DeskHelper.h"
#include "../include/SystemInfoTools.h"
#include <shlobj.h>

using namespace CDESK;
template<> CDeskHelper* SSingleton<CDeskHelper>::ms_Singleton = 0;

std::vector<stDeskIcon *> CDeskHelper::getDeskAllFiles()
{
	std::vector<stDeskIcon *> filesList;
	int nIsXp = SystemInfoTools::isXp();
	bool bIs64 = SystemInfoTools::is64();

	// 获取系统信息出错
	if (nIsXp == -1)
	{
		// TRACE("");
		return filesList;
	}

	WIN32_FIND_DATAA findFileData;
	char szdeskPath[2][MAX_PATH] = {0};

	SHGetSpecialFolderPathA(0, szdeskPath[0], CSIDL_DESKTOPDIRECTORY, 0);         // 私有目录
	SHGetSpecialFolderPathA(0, szdeskPath[1], CSIDL_COMMON_DESKTOPDIRECTORY, 0);  // 共有目录

	for (int i=0; i<2; i++)
	{
		std::string strDeskPath = szdeskPath[i];
		strDeskPath += "\\*";
		HANDLE hFind = FindFirstFileA(strDeskPath.c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			continue;
		}

		// 循环查找文件
		while (FindNextFileA(hFind, &findFileData) != 0)
		{
			// 排除根文件和隐藏文件
			if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY & ~FILE_ATTRIBUTE_HIDDEN)!=0
				&& strcmp(findFileData.cFileName, ".")==0
				|| strcmp(findFileData.cFileName, "..")==0)
			{
				continue;
			}

			std::string fileName = szdeskPath[i];
			fileName += std::string("\\") + findFileData.cFileName;

		}
	}

	return filesList;
}