#include "vd_system_logic.h"
#include "../command/vd_command.h"
#include "../file/vd_directory.h"
#include "../file/vd_link_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "../vd_enum.h"
#include <algorithm>
#include <iostream>
#include <regex>

VdSystemLogic::~VdSystemLogic()
{

}

VdSystemLogic::VdSystemLogic()
{

}

void VdSystemLogic::CreateRootDir()
{
	m_root = new VdDirectory("RootDir", DIR);
	VdDirectory* m_root_dir = dynamic_cast<VdDirectory*>(m_root);
	m_root_dir->SetRootFlag(true);
}

void VdSystemLogic::InitVdSystem()
{
	CreateRootDir();
	VdDirectory* disk_c = new VdDirectory("C:", DIR);
	VdDirectory* disk_d = new VdDirectory("D:", DIR);
	VdDirectory* disk_e = new VdDirectory("E:", DIR);
	disk_c->SetRootFlag(true);
	disk_d->SetRootFlag(true);
	disk_e->SetRootFlag(true);
	m_root->AddAbstractFile(disk_c);
	m_root->AddAbstractFile(disk_d);
	m_root->AddAbstractFile(disk_e);
	m_current_file_node = disk_c;
	m_active_file_node = disk_c;
}

void VdSystemLogic::PrintInitInfo(bool is_print_current_path /*= true*/)
{
	std::cout << "******************************************************************************" << std::endl;
	std::cout << "***                      Welcome to Virtual Disk System                    ***" << std::endl;
	std::cout << "*** There are three CDE disks in total, and the default is on the C drive. ***" << std::endl;
	std::cout << "*** You can use commands like dir/md/rd/cd/del/copy/ren/move/mklink/save.  ***" << std::endl;
	std::cout << "***                  (C)Jiantingwu Corporation. 保留所有权利.              ***" << std::endl;
	std::cout << "******************************************************************************" << std::endl;
	std::cout << std::endl;
	if (is_print_current_path)
	{
		m_current_file_node->PrintPath();
	}
}

void VdSystemLogic::HandleCommand(std::string input_cmd)
{
	std::string delimiter = " ";
	SplitCommandString(input_cmd, delimiter);
	VdCommand* command = nullptr;
	std::string command_name = GetCommandName();
	if (!command_name.empty())
	{
		command = RegisterCommandFactory::GetInstance().GetCommandByName(command_name);
	}

	if (command == nullptr)
	{
		if (!command_name.empty())
		{
			std::cout << "' " << command_name << " '" << " 不是内部命令。" << std::endl;
		}
		std::cout << std::endl;
		return;
	}

	if (m_command_para.size() == 2 && m_command_para[1] == "/?")
	{
		std::cout << std::endl;
		command->PrintHelp();
		std::cout << std::endl;
		return;
	}

	command->ClearParameter();
	command->Execute(this);
}


void VdSystemLogic::PrintCurrentPath()
{
	VdDirectory* dir = dynamic_cast<VdDirectory*>(m_current_file_node);
	if (dir->IsRootDir())
	{
		std::cout << m_current_file_node->GetCurrentPath() + "\\>";
		return;
	}
	std::cout << m_current_file_node->GetCurrentPath() + ">";
}

VdAbstractFile* VdSystemLogic::GetCurrentFile() const
{
	return  m_current_file_node;
}

void VdSystemLogic::SetCurrentFile(VdAbstractFile* current_file)
{
	m_current_file_node = current_file;
}

VdAbstractFile* VdSystemLogic::GetActiveFile() const
{
	return m_active_file_node;
}

void VdSystemLogic::SetActiveFile(VdAbstractFile* active_file)
{
	m_active_file_node = active_file;
}

void VdSystemLogic::SplitCommandString(const std::string& src, const std::string& delimiter)
{
	m_command_para.clear();
	m_command_para = VdTool::SplitStringBySpace(src);
	VdTool::ClearQuote(m_command_para);
}

std::string VdSystemLogic::GetCommandName()
{
	if (m_command_para.size() > 0)
	{
		return m_command_para[0];
	}
	return "";
}

bool VdSystemLogic::ChangeActiveDir(std::string dir_name)
{
	VdAbstractFile* current_file = this->GetCurrentFile();
	VdAbstractFile* active_file = this->GetActiveFile();
	VdDirectory* active_dir = dynamic_cast<VdDirectory*>(active_file);
	if (active_file->GetAbstractFileType() != DIR || active_dir == nullptr)
	{
		this->SetCurrentFile(current_file);
		this->SetActiveFile(current_file);
		std::cout << "目标路径不是一个目录，无法进行切换！" << std::endl;
		return false;
	}
	std::vector<VdAbstractFile*> sub_file_list = active_dir->GetSubFileList();
	bool exist = false;

	int file_num = (int)sub_file_list.size();
	for (int i = 0; i < file_num; i++)
	{
		if (sub_file_list[i]->GetAbstractFileType() == DIR && sub_file_list[i]->GetAbstractFileName() == dir_name)
		{
			if (dir_name == ".")
			{
				this->SetActiveFile(active_file);
			}
			else if (dir_name == "..")
			{
				if (active_dir->IsRootDir())
				{
					this->SetActiveFile(active_file);
				}
				else
				{
					this->SetActiveFile(active_file->GetParent());
				}
			}
			else
			{
				this->SetActiveFile(sub_file_list[i]);
			}
			exist = true;
			break;
		}
	}
	if (!exist)
	{
		this->SetCurrentFile(current_file);
		this->SetActiveFile(current_file);
		std::cout << "系统找不到指定路径，或目录名称无效。" << std::endl;
		return false;
	}
	return true;
}

VdAbstractFile* VdSystemLogic::GetFileByPath(const std::vector<std::string>& path)
{
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(GetActiveFile());
	if (!current_dir || path.size() == 0)
	{
		return nullptr;
	}
	VdAbstractFile* active_file = current_dir;
	bool successed = true;
	for (auto iter : path)
	{
		std::vector<VdAbstractFile*> sub_file_list;
		VdDirectory* active_dir = nullptr;
		if (active_file->GetAbstractFileType() == DIR)
		{
			active_dir = dynamic_cast<VdDirectory*>(active_file);
			sub_file_list = active_dir->GetSubFileList();
		}
		bool exist = false;
		for (auto file_iter : sub_file_list)
		{
			if (file_iter->GetAbstractFileName() == iter && iter == "..")
			{
				active_file = active_file->GetParent();
				exist = true;
				break;
			}
			else if (file_iter->GetAbstractFileName() == iter && iter == ".")
			{
				exist = true;
				break;
			}
			else if (file_iter->GetAbstractFileName() == iter)
			{
				exist = true;
				active_file = file_iter;
				break;
			}
		}
		if (!exist)
		{
			successed = false;
			break;
		}
	}
	if (!successed)
	{
		return nullptr;
	}
	return active_file;
}

bool VdSystemLogic::GetFileByPath(std::vector<VdAbstractFile*>& file_list, const std::vector<std::string>& path)
{
	std::vector<std::string> file_path(path.begin(), path.end());
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(GetActiveFile());
	int path_len = (int)file_path.size();
	if (!current_dir || path_len == 0)
	{
		return false;
	}

	//通配符问题
	std::string	end_path = *(path.end() - 1);
	VdTool::BuildRegexByWildcards(end_path);
	file_path[path_len - 1] = end_path;

	VdAbstractFile* active_file = current_dir;
	bool successed = true;
	int index = 0;
	for (auto iter : file_path)
	{
		index++;
		std::vector<VdAbstractFile*> sub_file_list;
		VdDirectory* active_dir = nullptr;
		if (active_file->GetAbstractFileType() == DIR)
		{
			active_dir = dynamic_cast<VdDirectory*>(active_file);
			sub_file_list = active_dir->GetSubFileList();
		}
		bool exist = false;
		std::regex regex(iter);
		for (auto file_iter : sub_file_list)
		{
			if (file_iter->GetAbstractFileName() == iter && iter == "..")
			{
				active_file = active_file->GetParent();
				exist = true;
				break;
			}
			else if (file_iter->GetAbstractFileName() == iter && iter == ".")
			{
				exist = true;
				break;
			}
			else if (std::regex_match(file_iter->GetAbstractFileName(), regex))
			{
				if (file_iter->GetAbstractFileName() == "." ||
					file_iter->GetAbstractFileName() == "..")
				{
					continue;
				}
				exist = true;
				active_file = file_iter;
				if (index == path_len)
				{
					file_list.emplace_back(active_file);
					continue;
				}
				break;
			}
		}
		if (!exist)
		{
			successed = false;
			break;
		}
	}
	if (!successed)
	{
		return false;
	}
	return true;
	
}

void VdSystemLogic::PostHandleLinkFile(VdLinkFile* link_file)
{
	if (link_file == nullptr)
	{
		return;
	}
	std::string file_path = link_file->GetLinkFilePath();
	std::vector<std::string> path = VdTool::SplitString(file_path, "\\");
	path.erase(path.begin());
	VdAbstractFile* file = GetFileByPath(path);
	link_file->SetLinkFile(file);
}

VdSystemLogic* VdSystemLogic::GetVdSystem()
{
	static VdSystemLogic vd_system;
	return &vd_system;
}
