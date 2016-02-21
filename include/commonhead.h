#ifndef COMMONHEAD_H
#define COMMONHEAD_H

#include <Windows.h>
#include <string>

/**< 桌面文件描述 */
struct stDeskIcon
{
	std::string fileName; /**< 文件名 */
	std::string filePath; /**< 文件路径 */
	HBITMAP hBitmap;      /**< 文件图标 */
};

#endif //