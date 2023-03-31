#include "vd_command.h"
#include "../logic/vd_system_logic.h"
#include "../vd_enum.h"
#include "../tool/vd_tool.h"
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

void VdCommand::PrintAddFileResult(int result_type, int is_dir)
{
	if (result_type == PATHTOOLONG)
	{
		std::string temp = is_dir ? "文件夹" : "文件";
		std::cout << temp << "路径已超过" << MAX_PATH_LENGTH << "个字符，请重试。" << std::endl;
		return;
	}
	if (result_type == EXISTSAMENAMEFILE)
	{
		std::cout << "已经存在同名文件（夹），无法操作。" << std::endl;
		return;
	}
}

