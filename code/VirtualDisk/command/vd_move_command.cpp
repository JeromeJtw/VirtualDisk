#include "vd_move_command.h"
#include "../file/vd_abstract_file.h"
#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include <algorithm>
#include <iostream>

VdMoveCommand::VdMoveCommand()
{

}

VdMoveCommand::~VdMoveCommand()
{
	ClearParameter();
}

void VdMoveCommand::ClearParameter()
{
	m_is_override = false;
	m_src_file_string = "";
	m_dst_file_string = "";
	m_src_path.clear();
	m_dst_path.clear();
	VdTool::SafeSetNull(m_src_file);
	VdTool::SafeSetNull(m_dst_file);
}

bool VdMoveCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_parameter = vd_system->GetCommandPara();
	std::vector<std::string>::iterator iter = std::find(command_parameter.begin(), command_parameter.end(), "/y");
	if (iter != command_parameter.end())
	{
		m_is_override = true;
		command_parameter.erase(iter);
	}
	command_parameter.erase(command_parameter.begin());
	if (command_parameter.size() < 2)
	{
		return false;
	}
	m_src_file_string = command_parameter[0];
	m_dst_file_string = command_parameter[1];
	return true;
}

void VdMoveCommand::Execute(VdSystemLogic* vd_system)
{
	if (vd_system == nullptr)
	{
		return;
	}
	PreExecute();
	if (!ParseParameter(vd_system))
	{
		std::cout << "命令语法错误，请重新输入！" << std::endl;
		return;
	}
	MoveFile(vd_system);
	PostExecute(vd_system);
}

void VdMoveCommand::PrintHelp()
{
	std::cout << "移动文件并重命名文件和目录。" << std::endl;
}

void VdMoveCommand::MoveFile(VdSystemLogic* vd_system)
{
	std::replace(m_src_file_string.begin(), m_src_file_string.end(), '/', '\\');
	std::replace(m_dst_file_string.begin(), m_dst_file_string.end(), '/', '\\');
	m_src_path = VdTool::SplitString(m_src_file_string, "\\");
	m_dst_path = VdTool::SplitString(m_dst_file_string, "\\");
	m_src_file = vd_system->GetFileByPath(m_src_path);
	if (m_src_file == nullptr)
	{
		std::cout << "系统无法找到指定文件" << std::endl;
		return;
	}

	if (m_src_file->GetAbstractFileType() == DIR)
	{
		MoveDir(vd_system);
	}
	else if (m_src_file->GetAbstractFileType() == NORMALFILE || m_src_file->GetAbstractFileType() == LINKFILE)
	{
		MoveNormalFile(vd_system);
	}
}

void VdMoveCommand::MoveDir(VdSystemLogic* vd_system)
{
	std::string new_file_name = "";
	if (m_dst_path.size() > 1)
	{
		std::vector<std::string> temp_dst_path(m_dst_path.begin(), m_dst_path.end() - 1);
		m_dst_file = vd_system->GetFileByPath(temp_dst_path);
		if (m_dst_file == nullptr)
		{
			std::cout << "系统无法找到指定文件。" << std::endl;
			return;
		}
		new_file_name = *(m_dst_path.end() - 1);
	}
	else if (m_dst_path.size() == 1)
	{
		m_dst_file = vd_system->GetCurrentFile();
		new_file_name = m_dst_path[0];
	}

	if (!VdTool::IsVaildDirName(new_file_name))
	{
		std::cout << "文件名、目录名语法不正确。" << std::endl;
		return;
	}

	VdDirectory* dst_dir = dynamic_cast<VdDirectory*>(m_dst_file);
	if (dst_dir->IsExistFile(new_file_name) && !m_is_override)
	{
		std::cout << "存在相同名字的文件无法移动" << std::endl;
		return;
	}

	VdDirectory* new_parent = dynamic_cast<VdDirectory*>(m_dst_file);
	VdDirectory* old_parent = dynamic_cast<VdDirectory*>(m_src_file->GetParent());
	old_parent->EraseSubFileByName(m_src_file->GetAbstractFileName());
	m_src_file->ReName(new_file_name);
	//ovveride 确保目标路径中同名文件夹销毁
	VdDirectory* dir = dynamic_cast<VdDirectory*>(new_parent->GetSubFileByName(new_file_name));
	if (dir)
	{
		dir->RecursionDestoryDir();
	}
	new_parent->RecursionDeleteSubFileByName(new_file_name, false);
	
	new_parent->AddAbstractFile(m_src_file);
	std::cout << "移动了		" << 1 << "个文件夹" << std::endl;
}

void VdMoveCommand::MoveNormalFile(VdSystemLogic* vd_system)
{
	std::string new_file_name = "";
	if (m_dst_path.size() > 1)
	{
		std::vector<std::string> temp_dst_path(m_dst_path.begin(), m_dst_path.end() - 1);
		m_dst_file = vd_system->GetFileByPath(temp_dst_path);
		if (m_dst_file == nullptr)
		{
			std::cout << "系统无法找到指定文件。" << std::endl;
			return;
		}
		new_file_name = *(m_dst_path.end() - 1);
	}
	else if (m_dst_path.size() == 1)
	{
		m_dst_file = vd_system->GetCurrentFile();
		new_file_name = m_dst_path[0];
	}

	if (!VdTool::IsVaildFileName(new_file_name))
	{
		std::cout << "文件名、目录名语法不正确。" << std::endl;
		return;
	}
	
	VdDirectory* dst_dir = dynamic_cast<VdDirectory*>(m_dst_file);
	if (dst_dir->IsExistFile(new_file_name) && !m_is_override)
	{
		std::cout << "存在相同名字的文件无法移动" << std::endl;
		return;
	}

	//override
	dst_dir->DeleteSubFileByName(new_file_name);
	VdFile* src_file = dynamic_cast<VdFile*>(m_src_file);
	VdFile* new_file = new VdFile(*src_file);
	new_file->ReName(new_file_name);

	VdDirectory* src_parent = dynamic_cast<VdDirectory*>(src_file->GetParent());
	src_parent->DeleteSubFileByName(src_file->GetAbstractFileName());

	dst_dir->AddAbstractFile(new_file);
	std::cout << "移动了		" << 1 << "个文件" << std::endl;
}
