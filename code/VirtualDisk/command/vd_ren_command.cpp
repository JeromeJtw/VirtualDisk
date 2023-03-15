#include "../logic/vd_system_logic.h"
#include "../file/vd_directory.h"
#include "../tool/vd_tool.h"
#include "vd_ren_command.h"
#include <algorithm>
#include <iostream>

VdRenCommand::VdRenCommand()
{

}

VdRenCommand::~VdRenCommand()
{
	ClearParameter();
}

void VdRenCommand::ClearParameter()
{
	m_src_file_name = "";
	m_src_path.clear();
	m_dst_file_name = "";
}

bool VdRenCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	if (command_para.size() < 3)
	{
		return false;
	}
	
	if (command_para[2].find("\\") != std::string::npos || command_para[2].find("/") != std::string::npos)
	{
		return false;
	}
	m_src_file_name = command_para[1];
	std::replace(m_src_file_name.begin(), m_src_file_name.end(), '/', '\\');
	m_src_path = VdTool::SplitString(m_src_file_name, "\\");
	m_dst_file_name = command_para[2];
	return true;
}

void VdRenCommand::Execute(VdSystemLogic* vd_system)
{
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
	ReName(vd_system);
	PostExecute(vd_system);
}

void VdRenCommand::PrintHelp()
{
	std::cout << "重命名文件或文件夹。" << std::endl;
}

void VdRenCommand::ReName(VdSystemLogic* vd_system)
{
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	if (current_dir == nullptr)
	{
		return;
	}
	if (m_src_file_name == "." || m_src_file_name == "..")
	{
		std::cout << "操作不合法！系统无法接受请求的路径或文件名，请对其他文件（夹）重命名！" << std::endl;
		return;
	}
	if (m_dst_file_name == "." || m_dst_file_name == "..")
	{
		std::cout << "操作不合法！请更换为非'.''..'的名字" << std::endl;
		return;
	}
	
	VdAbstractFile* src_file = vd_system->GetFileByPath(m_src_path);
	if (src_file == nullptr)
	{
		std::cout << "系统找不到指定路径！" << std::endl;
		return;
	}

	std::vector<VdAbstractFile*> file_list;
	VdDirectory* parent_dir = dynamic_cast<VdDirectory*>(src_file->GetParent());
	if (parent_dir->IsExistFile(m_dst_file_name))
	{
		std::cout << "已存在同名文件（夹），无法进行修改！" << std::endl;
		return;
	}
	src_file->ReName(m_dst_file_name);
}
