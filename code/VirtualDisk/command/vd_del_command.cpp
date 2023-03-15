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
		std::cout << "�����﷨����ȷ�����������룡" << std::endl;
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
	std::cout << "ɾ��һ�������ļ�����ɾ���ļ��С�" << std::endl;
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
			std::cout << "ϵͳ�޷��ҵ�" << iter << "�ļ���" << std::endl;
			continue;
		}

		for (auto delete_file : delete_file_list)
		{
			int result;
			bool is_dir = false;
			std::string file_path_string = delete_file->GetCurrentPath();
			if (delete_file->GetAbstractFileType() == DIR)
			{
				//ɾ������ļ����������е��ļ�
				VdDirectory* dir = dynamic_cast<VdDirectory*>(delete_file);
				dir->DeleteAllNormalFile();
				result = SUCCESSED;
				is_dir = true;
			}
			else
			{
				//ɾ������ļ�����
				VdDirectory* parent_dir = dynamic_cast<VdDirectory*>(delete_file->GetParent());
				result = parent_dir->DeleteSubFileByName(delete_file->GetAbstractFileName());
			}
			PrintResult(file_path_string, result, is_dir);
		}
	}
}

void VdDelCommand::RecursionDelete(VdSystemLogic* vd_system)
{
	//TODO���ݹ�ɾ���ļ�����ɾ���ļ���
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
				//ɾ������ļ����µ������ļ�
				VdDirectory* dir = dynamic_cast<VdDirectory*>(file);
				dir->RecursionDeleteAllFile();
				std::cout << "�ݹ�ɾ���� " << dir->GetCurrentPath() << " �ļ����������ļ�";
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
				//�ݹ�ɾ���ļ����ڵ��ļ�
				VdDirectory* dir = dynamic_cast<VdDirectory*>(file);
				dir->RecursionDeleteAllFile();
				std::cout << "�ݹ�ɾ���� " << dir->GetCurrentPath() << " �ļ����������ļ�";
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
			std::cout << "ɾ���ļ� - " << file_path << std::endl;
		}
		else
		{
			std::cout << "û���ҵ�Ŀ���ļ���" << std::endl;
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
			std::cout << "ɾ���ļ������ļ� - " << file_name << std::endl;
		}
		else
		{
			std::cout << "ɾ���ļ� - " << file_name << std::endl;
		}
		break;
	case CANNOTRM:
		std::cout << "' " << file_name << " '" << "�޷�ɾ����" << std::endl;
		break;
	case NOTFIND:
		std::cout << "ϵͳ�޷��ҵ�" << file_name << "�ļ���" << std::endl;
		break;
	default:
		break;
	}

}
