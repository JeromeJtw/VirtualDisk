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
		std::string temp = is_dir ? "�ļ���" : "�ļ�";
		std::cout << temp << "·���ѳ���" << MAX_PATH_LENGTH << "���ַ��������ԡ�" << std::endl;
		return;
	}
	if (result_type == EXISTSAMENAMEFILE)
	{
		std::cout << "�Ѿ�����ͬ���ļ����У����޷�������" << std::endl;
		return;
	}
}

