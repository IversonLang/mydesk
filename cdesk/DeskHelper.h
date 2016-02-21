#pragma once

#include <vector>
#include "../include/commonhead.h"
#include "core/SSingleton.h"
using namespace SOUI;

class CDeskHelper : public SSingleton<CDeskHelper>
{
public:
	CDeskHelper(void)
	{

	}
	~CDeskHelper(void)
	{

	}


public:
	// 获取桌面所有文件信息
	std::vector<stDeskIcon *> getDeskAllFiles();
};

