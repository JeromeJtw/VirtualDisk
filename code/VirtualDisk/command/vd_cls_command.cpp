#include "vd_cls_command.h"
#include "../logic/vd_system_logic.h"
#include <iostream>

VdClsCommand::VdClsCommand()
{

}

VdClsCommand::~VdClsCommand()
{

}

void VdClsCommand::Execute(VdSystemLogic* vd_system)
{
	system("cls");
	if (vd_system)
	{
		vd_system->PrintInitInfo(false);
	}
}


void VdClsCommand::PrintHelp()
{
	std::cout << "Çå³ýÆÁÄ»¡£" << std::endl;
}