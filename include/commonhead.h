#ifndef COMMONHEAD_H
#define COMMONHEAD_H

#include <Windows.h>
#include <string>

/**< �����ļ����� */
struct stDeskIcon
{
	std::string fileName; /**< �ļ��� */
	std::string filePath; /**< �ļ�·�� */
	HBITMAP hBitmap;      /**< �ļ�ͼ�� */
};

#endif //