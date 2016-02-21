#ifndef DESKCOLLATORFUN_H
#define DESKCOLLATORFUN_H

#include <string>

typedef void (*setWndInfoFun)(HWND hWnd);

typedef bool (*updateDeskBackgroundFun)(std::wstring &wallpaperPath, int &nStyle);

#endif