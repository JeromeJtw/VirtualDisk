#include "logic/vd_system_logic.h"
#include <iostream>

int main()
{
	VdSystemLogic* vd_system = VdSystemLogic::GetVdSystem();
	if (vd_system == nullptr)
	{
		return 0;
	}
	vd_system->InitVdSystem();
	vd_system->PrintInitInfo();
	char input_cmd[512];
	while (1)
	{
		std::cin.getline(input_cmd, 512);
		if (strcmp(input_cmd, "exit") == 0)
		{
			break;
		}

		vd_system->HandleCommand(input_cmd);
		vd_system->PrintCurrentPath();
	}
	return 0;
}