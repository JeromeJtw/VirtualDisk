#include "vd_md_command.h"
#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../clipp/clipp.h"
#include <iostream>

VdMdCommand::VdMdCommand()
{
	m_dir_name_list.clear();
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
		if (para.find('*') != std::string::npos || para.find('?') != std::string::npos || 
			para.find(':') != std::string::npos || para.find('<') != std::string::npos || 
			para.find('>') != std::string::npos || para.find('|') != std::string::npos || 
			para.find('.') != std::string::npos)
		{
			return false;
		}
	}
	return true;
}

void VdMdCommand::Execute(VdSystemLogic* vd_system)
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

	//2.���ݲ���ִ��
	MakeDir(vd_system);
	PostExecute(vd_system);
}

void VdMdCommand::PrintHelp()
{
	std::cout << "�����ļ��С�" << std::endl;
}

void VdMdCommand::MakeDir(VdSystemLogic* vd_system)
{
	VdAbstractFile* current_file = vd_system->GetCurrentFile();
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(current_file);
	if (current_dir == nullptr)
	{
		std::cout << "�޷������ļ��У�" << std::endl;
		return;
	}
	
	int make_dir_nums = (int)m_dir_name_list.size();
	for (int j = 0; j < make_dir_nums; j++)
	{
		if (m_dir_name_list[j].find("\\") != std::string::npos || m_dir_name_list[j].find("/") != std::string::npos)
		{
			std::cout << "'" << m_dir_name_list[j] << "'" << "�﷨����ȷ���޷�������" << std::endl;
			continue;
		}
		
		if (current_dir->IsExistFile(m_dir_name_list[j]))
		{
			std::cout << "'" << m_dir_name_list[j] << "'" << "�Ѿ����ڣ��޷�����ͬ���ļ��У�" << std::endl << std::endl;
			continue;
		}

		VdAbstractFile* dir = new VdDirectory(m_dir_name_list[j], DIR);
		if (dir == nullptr)
		{
			std::cout << "'" << m_dir_name_list[j] << "'" << "�ļ��д���ʧ��" << std::endl;
			return;
		}
		current_file->AddAbstractFile(dir);
		std::cout << "'" << m_dir_name_list[j] << "'" << "�ļ��д����ɹ�" << std::endl;
	}
}
