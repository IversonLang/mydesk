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
	// ��ȡ���������ļ���Ϣ
	std::vector<stDeskIcon *> getDeskAllFiles();
};

