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
			std::cout << "�ļ���������" << MAX_NAME_LENGTH << "���ַ�" << std::endl;
			return false;
		}
		if (res == HASINVAILDCHAR)
		{
			std::cout << para <<"�ļ������зǷ��ַ�" << std::endl;
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

	for (auto dir_name : m_dir_name_list)
	{
		if (dir_name.find("\\") != std::string::npos || dir_name.find("/") != std::string::npos)
		{
			std::cout << "'" << dir_name << "'" << "�﷨����ȷ���޷�������" << std::endl;
			continue;
		}

		VdAbstractFile* dir = new VdDirectory(dir_name, DIR);
		if (dir == nullptr)
		{
			std::cout << "'" << dir_name << "'" << "�ļ��д���ʧ��" << std::endl;
			return;
		}
		int res = current_dir->AddAbstractFile(dir);
		if (res != ADDSUCCESSED)
		{
			PrintAddFileResult(res,true);
			VdTool::SafeDeleteSetNull(dir);
			continue;
		}
		std::cout << "'" << dir_name << "'" << "�ļ��д����ɹ�" << std::endl;
	}
}
