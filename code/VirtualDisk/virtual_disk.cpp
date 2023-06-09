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
	std::string input_cmd;
	while (true)
	{
		std::getline(std::cin, input_cmd);
		if (input_cmd.size() > 512)
		{
			std::cout << std::endl << "命令长度大于512，无法处理,请重新输入" << std::endl;
			vd_system->PrintCurrentPath();
			continue;
		}
		if (strcmp(input_cmd.c_str(), "exit") == 0)
		{
			vd_system->DestroyVdSystem();
			break;
		}

		vd_system->HandleCommand(input_cmd);
		vd_system->PrintCurrentPath();
	}
	return 0;
}