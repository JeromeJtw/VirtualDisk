#include "../file/vd_directory.h"
#include "../file/vd_link_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_mklink_command.h"
#include <algorithm>
#include <iostream>

VdMklinkCommand::VdMklinkCommand()
{

}

VdMklinkCommand::~VdMklinkCommand()
{

}

void VdMklinkCommand::ClearParameter()
{
	m_src_file_string = "";
	m_link_string = "";
	m_src_file = nullptr;
}

bool VdMklinkCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	if (command_para.size() < 3)
	{
		return false;
	}
	m_src_file_string = command_para[1];
	m_link_string = command_para[2];
	return true;
}

void VdMklinkCommand::Execute(VdSystemLogic* vd_system)
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
	if (strcmp(m_src_file_string.c_str(), m_link_string.c_str()) == 0)
	{
		std::cout << "拒绝访问！" << std::endl;
		return;
	}
	MakeLink(vd_system);
	PostExecute(vd_system);
}


void VdMklinkCommand::PrintHelp()
{
	std::cout << "创建符号链接。" << std::endl;
}

void VdMklinkCommand::MakeLink(VdSystemLogic* vd_system)
{
	std::replace(m_src_file_string.begin(), m_src_file_string.end(), '/', '\\');
	std::vector<std::string> src_path = VdTool::SplitString(m_src_file_string, "\\");
	m_src_file = vd_system->GetFileByPath(src_path);
	if (m_src_file == nullptr)
	{
		std::cout << "系统找不到指定的文件（夹）" << std::endl;
		return;
	}

	std::replace(m_link_string.begin(), m_link_string.end(), '/', '\\');
	std::vector<std::string> dst_path = VdTool::SplitString(m_link_string, "\\");
	std::string link_file_name = *(dst_path.end() - 1);
	if (!VdTool::IsVaildFileName(link_file_name))
	{
		std::cout << "文件名、目录名语法不正确。" << std::endl;
		return;
	}

	VdDirectory* dst_dir;
	if (dst_path.size() == 1)
	{
		dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	}
	else
	{
		std::vector<std::string> dst_dir_path(dst_path.begin(), dst_path.end() - 1);
		dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetFileByPath(dst_dir_path));
	}

	if (dst_dir == nullptr)
	{
		std::cout << "系统找不到指定的路径！" << std::endl;
		return;
	}

	VdAbstractFile* dst_file = dst_dir->GetSubFileByName(link_file_name);
	if (dst_file && dst_file->GetAbstractFileType() == LINKFILE)
	{
		std::cout << "当文件已存在时，无法创建该符号链接！" << std::endl;
		return;
	}

	//创建链接
	VdLinkFile* link_file = new VdLinkFile(link_file_name,LINKFILE);
	link_file->SetLinkFile(m_src_file);
	dst_dir->AddAbstractFile(link_file);
	std::cout << "为" << m_src_file_string << "<<===>>" << m_link_string << "创建符号链接" << std::endl;
}
