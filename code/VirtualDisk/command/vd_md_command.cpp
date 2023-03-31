#include "vd_md_command.h"
#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "../clipp/clipp.h"
#include <iostream>

VdMdCommand::VdMdCommand()
{

}

VdMdCommand::~VdMdCommand()
{

}

void VdMdCommand::ClearParameter()
{
	m_dir_name_list.clear();
}

bool VdMdCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_parameter = vd_system->GetCommandPara();
	m_dir_name_list.insert(m_dir_name_list.end(), command_parameter.begin() + 1, command_parameter.end());
	if (m_dir_name_list.size() < 0)
	{
		return false;
	}
	for (auto para : m_dir_name_list)
	{
		int res = VdTool::IsVaildDirName(para);
		if (res == TOOLONG)
		{
			std::cout << "文件夹名超过" << MAX_NAME_LENGTH << "个字符" << std::endl;
			return false;
		}
		if (res == HASINVAILDCHAR)
		{
			std::cout << para <<"文件夹名有非法字符" << std::endl;
			return false;
		}
	}
	return true;
}

void VdMdCommand::Execute(VdSystemLogic* vd_system)
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

	//2.根据参数执行
	MakeDir(vd_system);
	PostExecute(vd_system);
}

void VdMdCommand::PrintHelp()
{
	std::cout << "创建文件夹。" << std::endl;
}

void VdMdCommand::MakeDir(VdSystemLogic* vd_system)
{
	VdAbstractFile* current_file = vd_system->GetCurrentFile();
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(current_file);
	if (current_dir == nullptr)
	{
		std::cout << "无法创建文件夹！" << std::endl;
		return;
	}

	for (auto dir_name : m_dir_name_list)
	{
		if (dir_name.find("\\") != std::string::npos || dir_name.find("/") != std::string::npos)
		{
			std::cout << "'" << dir_name << "'" << "语法不正确，无法创建！" << std::endl;
			continue;
		}

		VdAbstractFile* dir = new VdDirectory(dir_name, DIR);
		if (dir == nullptr)
		{
			std::cout << "'" << dir_name << "'" << "文件夹创建失败" << std::endl;
			return;
		}
		int res = current_dir->AddAbstractFile(dir);
		if (res != ADDSUCCESSED)
		{
			PrintAddFileResult(res,true);
			VdTool::SafeDeleteSetNull(dir);
			continue;
		}
		std::cout << "'" << dir_name << "'" << "文件夹创建成功" << std::endl;
	}
}
