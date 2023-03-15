#include "vd_command.h"
#include "../logic/vd_system_logic.h"
#include <iostream>

VdCommand::VdCommand()
{

}

VdCommand::~VdCommand()
{

}

bool VdCommand::ParseParameter(VdSystemLogic* vd_system)
{
	return true;
}

void VdCommand::ClearParameter()
{

}

void VdCommand::PreExecute()
{
	std::cout << std::endl;
}

void VdCommand::Execute(VdSystemLogic* vd_system)
{
	std::cout << __FUNCTION__ << std::endl;
}

void VdCommand::PostExecute(VdSystemLogic* vd_system)
{
	std::cout << std::endl;
	vd_system->SetActiveFile(vd_system->GetCurrentFile());
}

void VdCommand::PrintHelp()
{
	std::cout << __FUNCTION__ << std::endl;
}


