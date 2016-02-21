#pragma once

namespace CDESK
{
	class SystemInfoTools
	{
	public:
		SystemInfoTools(void) {}
		~SystemInfoTools(void) {}

	public:
		// return -1:³ö´í  1:XP  0:·ÇXP
		static int isXp();

		static bool is64();
	};
}



