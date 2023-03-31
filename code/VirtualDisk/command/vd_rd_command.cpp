#pragma once
#include "../clipp/clipp.h"
#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "vd_rd_command.h"
#include <algorithm>
#include <iostream>
static const std::string RDRECURSIONFLAG = "/s";

VdRdCommand::VdRdCommand()
{
	m_dst_file_list.clear();
}

VdRdCommand::~VdRdCommand()
{

}

void VdRdCommand::Execute(VdSystemLogic* vd_system)
{
	//1.��������
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
	if (!m_is_recursion_delete)
	{
		RemoveDir(vd_system);
	}
	else
	{
		RecursionRemoveDir(vd_system);
	}
	
	PostExecute(vd_system);
}

bool VdRdCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_parameter = vd_system->GetCommandPara();
	auto iter = std::find(command_parameter.begin(), command_parameter.end(), RDRECURSIONFLAG);
	if (iter != command_parameter.end())
	{
		m_is_recursion_delete = true;
		command_parameter.erase(iter);
	}
	m_dst_file_list.insert(m_dst_file_list.end(), command_parameter.begin() + 1, command_parameter.end());
	if (m_dst_file_list.size() > 0)
	{
		return true;
	}
	return false;
}

void VdRdCommand::PrintHelp()
{
	std::cout << "ɾ��һ��Ŀ¼��" << std::endl;
}

void VdRdCommand::ClearParameter()
{
	m_is_recursion_delete = false;
	m_dst_file_list.clear();
}

bool VdRdCommand::RemoveDir(VdSystemLogic* vd_system)
{
	VdDirectory* active_dir = dynamic_cast<VdDirectory*>(vd_system->GetActiveFile());
	if (active_dir == nullptr)
	{
		return false;
	}
	for (auto dst_file : m_dst_file_list)
	{
		int result = active_dir->RemoveSubDirByName(dst_file);
		switch (result)
		{
		case NOTEMPTY:
			std::cout << "' " << dst_file << " '" << "���ǿ�Ŀ¼��" << std::endl;
			break;
		case ISUSING:
			std::cout << "' " << dst_file << " '" << "����ʹ�á�" << std::endl;
			break;
		case CANNOTRM:
			std::cout << "' " << dst_file << " '" << "�޷�ɾ����" << std::endl;
			break;
		case NOTFIND:
			std::cout << "ϵͳ�޷��ҵ�" << dst_file << "�ļ��С�" << std::endl;
			break;
		case SUCCESSED:
			std::cout << dst_file << "�ļ���ɾ���ɹ���" << std::endl;
			break;
		default:
			std::cout << "δ֪����" << std::endl;
			break;
		}
	}
	return true;
}

void VdRdCommand::RecursionRemoveDir(VdSystemLogic* vd_system)
{
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	
	for (auto iter : m_dst_file_list)
	{
		std::vector<VdAbstractFile*> file_list = current_dir->GetNormalSubFileList();
		bool is_found = false;
		for (auto file_iter : file_list)
		{
			if (file_iter->GetAbstractFileType() == DIR && file_iter->GetAbstractFileName() == iter)
			{
				VdDirectory* dir = dynamic_cast<VdDirectory*>(file_iter);
				dir->RecursionDeleteAllFile();
				current_dir->RemoveSubDir(dir);
				is_found = true;
			}
		}
		if (!is_found)
		{
			std::cout << "ϵͳ�Ҳ���' " << iter << " '�ļ���" << std::endl;
		}
	}
}
