#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_cd_command.h"
#include <algorithm>
#include <iostream>


VdCdCommand::VdCdCommand()
{

}

VdCdCommand::~VdCdCommand()
{

}

void VdCdCommand::ClearParameter()
{
	m_dst_path = "";
}

bool VdCdCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_parameter = vd_system->GetCommandPara();
	if (command_parameter.size() > 1)
	{
		m_dst_path = command_parameter[1];
	}
	return true;
}

void VdCdCommand::Execute(VdSystemLogic* vd_system)
{
	//1.解析参数
	if (vd_system == nullptr)
	{
		return;
	}
	PreExecute();
	if (!ParseParameter(vd_system))
	{
		std::cout << "命令语法不正确，请重新输入！" << std::endl;
		return;
	}
	if (m_dst_path.empty())
	{
		vd_system->PrintCurrentPath();
		std::cout << std::endl;
	}
	else
	{
		//解析path，并切换m_current_file_node
		std::replace(m_dst_path.begin(), m_dst_path.end(), '/', '\\');
		std::vector<std::string> dst_path_list = VdTool::SplitString(m_dst_path, "\\");
		bool succeeded = true;
		for (auto iter : dst_path_list)
		{
			if (!ChangeCurrentDir(vd_system, iter))
			{
				succeeded = false;
				break;
			}
		}
		if (succeeded)
		{
			vd_system->SetCurrentFile(vd_system->GetActiveFile());
		}
	}
	PostExecute(vd_system);
}

void VdCdCommand::PrintHelp()
{
	std::cout << "显示当前目录名或改变当前目录。" << std::endl;
}

bool VdCdCommand::ChangeCurrentDir(VdSystemLogic* vd_system, const std::string& dir_name)
{
	return vd_system->ChangeActiveDir(dir_name);
}
