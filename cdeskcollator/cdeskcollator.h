#ifndef CDESKCOLLATOR_H
#define CDESKCOLLATOR_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "../include/registry.h"

void init();

void uninit();

SIZE getAvailableDeskSize();

SIZE getDeskScreenSize();

void setWndInfo(HWND hWnd);

bool updateDeskBackground(std::wstring &wallpaperPath, int &nStyle);


#endif //