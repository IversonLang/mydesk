#pragma once

namespace CDESK
{
	class SystemInfoTools
	{
	public:
		SystemInfoTools(void) {}
		~SystemInfoTools(void) {}

	public:
		// return -1:����  1:XP  0:��XP
		static int isXp();

		static bool is64();
	};
}



