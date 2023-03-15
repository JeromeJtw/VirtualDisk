#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_del_command.h"
#include <algorithm>
#include <iostream>

VdDelCommand::VdDelCommand()
{

}

VdDelCommand::~VdDelCommand(){

}

bool VdDelCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	auto iter = find(command_para.begin(), command_para.end(), "/s");
	if (iter != command_para.end())
	{
		m_recursion_delete = true;
		m_dst_file_list.insert(m_dst_file_list.end(), command_para.begin() + 2, command_para.end());
	}
	else
	{
		m_recursion_delete = false;
		m_dst_file_list.insert(m_dst_file_list.end(), command_para.begin() + 1, command_para.end());
	}
	if (m_dst_file_list.size() == 0)
	{
		return false;
	}
	auto all_iter = find(m_dst_file_list.begin(), m_dst_file_list.end(), ".");
	if (all_iter != m_dst_file_list.end())
	{
		m_delete_all_file = true;
	}
	return true;
}

void VdDelCommand::ClearParameter()
{
	m_recursion_delete = false;
	m_delete_all_file = false;
	m_dst_file_list.clear();
}

void VdDelCommand::Execute(VdSystemLogic* vd_system)
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
	if (!m_recursion_delete)
	{
		DeleteFile(vd_system);
	}
	else
	{
		RecursionDelete(vd_system);
	}
	PostExecute(vd_system);
}

void VdDelCommand::PrintHelp()
{
	std::cout << "删除一个或多个文件。不删除文件夹。" << std::endl;
}

void VdDelCommand::DeleteFile(VdSystemLogic* vd_system)
{
	VdDirectory* active_dir = dynamic_cast<VdDirectory*>(vd_system->GetActiveFile());
	if (active_dir == nullptr)
	{
		return;
	}
	if (m_delete_all_file)
	{
		active_dir->DeleteAllNormalFile();
		return;
	}
	for (auto iter : m_dst_file_list)
	{
		std::replace(iter.begin(), iter.end(), '/', '\\');
		std::vector<std::string> file_path = VdTool::SplitString(iter,"\\");
		std::vector<VdAbstractFile*> delete_file_list;
		if (!vd_system->GetFileByPath(delete_file_list, file_path))
		{
			std::cout << "系统无法找到" << iter << "文件。" << std::endl;
			continue;
		}

		for (auto delete_file : delete_file_list)
		{
			int result;
			bool is_dir = false;
			std::string file_path_string = delete_file->GetCurrentPath();
			if (delete_file->GetAbstractFileType() == DIR)
			{
				//删除这个文件夹下面所有的文件
				VdDirectory* dir = dynamic_cast<VdDirectory*>(delete_file);
				dir->DeleteAllNormalFile();
				result = SUCCESSED;
				is_dir = true;
			}
			else
			{
				//删除这个文件即可
				VdDirectory* parent_dir = dynamic_cast<VdDirectory*>(delete_file->GetParent());
				result = parent_dir->DeleteSubFileByName(delete_file->GetAbstractFileName());
			}
			PrintResult(file_path_string, result, is_dir);
		}
	}
}

void VdDelCommand::RecursionDelete(VdSystemLogic* vd_system)
{
	//TODO：递归删除文件，不删除文件夹
	for (auto iter : m_dst_file_list)
	{
		std::replace(iter.begin(), iter.end(), '/', '\\');
		std::vector<std::string> path = VdTool::SplitString(iter, "\\");

		if (path.size() == 0)
		{
			continue;
		}
		else if (path.size() == 1)
		{
			VdTool::BuildRegexByWildcards(path[0]);
			VdAbstractFile* file = vd_system->GetFileByPath(path);
			if (file && file->GetAbstractFileType() == DIR)
			{
				//删除这个文件夹下的所有文件
				VdDirectory* dir = dynamic_cast<VdDirectory*>(file);
				dir->RecursionDeleteAllFile();
				std::cout << "递归删除了 " << dir->GetCurrentPath() << " 文件夹内所有文件";
			}
			else
			{
				VdDirectory* current_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
				RecursionDeleteFile(vd_system, current_dir, path[0]);
			}
			
		}
		else
		{
			VdAbstractFile* file = vd_system->GetFileByPath(path);
			if (file && file->GetAbstractFileType() == DIR)
			{
				//递归删除文件夹内的文件
				VdDirectory* dir = dynamic_cast<VdDirectory*>(file);
				dir->RecursionDeleteAllFile();
				std::cout << "递归删除了 " << dir->GetCurrentPath() << " 文件夹内所有文件";
			}
			else
			{
				std::string vague_name = *(path.end() - 1);
				VdTool::BuildRegexByWildcards(vague_name);
				path.erase(path.end() - 1);
				VdDirectory* work_dir = dynamic_cast<VdDirectory*>(vd_system->GetFileByPath(path));
				RecursionDeleteFile(vd_system, work_dir, vague_name);
			}
		}
	}
}

void VdDelCommand::RecursionDeleteFile(VdSystemLogic* vd_system, VdDirectory* work_dir, const std::string vague_name)
{
	if (work_dir == nullptr)
	{
		return;
	}
	std::vector<VdAbstractFile*> delete_file_list = work_dir->GetNormalSubFileByVagueName(vague_name);
	for (auto delete_file : delete_file_list)
	{
		std::string file_path = delete_file->GetCurrentPath();
		if (work_dir->DeleteSubFileByName(delete_file->GetAbstractFileName()) == SUCCESSED)
		{
			std::cout << "删除文件 - " << file_path << std::endl;
		}
		else
		{
			std::cout << "没有找到目标文件。" << std::endl;
		}
	}

	std::vector<VdDirectory*> sub_dir = work_dir->GetDir();
	for (auto dir : sub_dir)
	{
		RecursionDeleteFile(vd_system, dir, vague_name);
	}
}

void VdDelCommand::PrintResult(const std::string file_name, const int result, const bool is_dir)
{
	switch (result)
	{
	case SUCCESSED:
		if (is_dir)
		{
			std::cout << "删除文件夹内文件 - " << file_name << std::endl;
		}
		else
		{
			std::cout << "删除文件 - " << file_name << std::endl;
		}
		break;
	case CANNOTRM:
		std::cout << "' " << file_name << " '" << "无法删除。" << std::endl;
		break;
	case NOTFIND:
		std::cout << "系统无法找到" << file_name << "文件。" << std::endl;
		break;
	default:
		break;
	}

}
