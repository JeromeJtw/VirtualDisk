#include "../clipp/clipp.h"
#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_mf_command.h"
#include <iostream>

VdMfCommand::VdMfCommand()
{

}

VdMfCommand::~VdMfCommand()
{

}

void VdMfCommand::ClearParameter()
{
	m_file_name = "";
	m_file_size = 0;
}

bool VdMfCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_parameter = vd_system->GetCommandPara();
	command_parameter.insert(command_parameter.begin(), "placeholdersstring");
	int n = (int)command_parameter.size();
	const char* parameters[100];
	for (int i = 0; i < n; ++i)
	{
		parameters[i] = command_parameter[i].c_str();
	}
	std::string command_name;
	auto cli = (clipp::value("command_name", command_name),
		clipp::value("file_name", m_file_name),
		clipp::value("file_size", m_file_size));
	if (clipp::parse(n, const_cast<char**>(parameters), cli))
	{
		return true;
	}
	else
		return false;
}

void VdMfCommand::Execute(VdSystemLogic* vd_system)
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
	CreateFile(vd_system);
	PostExecute(vd_system);
}

void VdMfCommand::PrintHelp()
{
	std::cout << "�����ļ���" << std::endl;
}

void VdMfCommand::CreateFile(VdSystemLogic* vd_system)
{
	VdDirectory* current_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	if (current_dir == nullptr)
	{
		return;
	}

	if (m_file_name.find("\\") != std::string::npos || m_file_name.find("/") != std::string::npos)
	{
		std::cout << "�ļ������ܰ���'\\'��'/'" << std::endl;
		return;
	}

	int res = VdTool::IsVaildFileName(m_file_name);
	if (res == TOOLONG)
	{
		std::cout << "Ŀ���ļ�������" << MAX_NAME_LENGTH << "���ַ����޷�������" << std::endl;
		return;
	}
	if (res == HASINVAILDCHAR)
	{
		std::cout << "�ļ��������Ƿ��ַ����޷�������" << std::endl;
		return;
	}

	VdAbstractFile* new_file = new VdFile(m_file_name, NORMALFILE, m_file_size);
	int add_res = current_dir->AddAbstractFile(new_file);
	if (add_res != ADDSUCCESSED)
	{
		VdTool::SafeDeleteSetNull(new_file);
		PrintAddFileResult(add_res, false);
		return;
	}
	std::cout << "�ļ�"<<m_file_name << "�����ɹ�" << std::endl;
}
