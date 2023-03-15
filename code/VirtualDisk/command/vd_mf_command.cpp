#include "../clipp/clipp.h"
#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../logic/vd_system_logic.h"
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
	for (int i = 0; i < n; i++)
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
	//1.解析参数
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
	CreateFile(vd_system);
	PostExecute(vd_system);
}

void VdMfCommand::PrintHelp()
{
	std::cout << "创建文件。" << std::endl;
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
		std::cout << "文件名不能包含'\\'或'/'" << std::endl;
		return;
	}

	std::vector<VdAbstractFile*> file_list = current_dir->GetNormalSubFileList();
	for (auto iter : file_list)
	{
		if (iter->GetAbstractFileName() == m_file_name && iter->GetAbstractFileType() == NORMALFILE)
		{
			std::cout << "'" << m_file_name << "'" << "已经存在，不能创建名字相同的文件！" << std::endl;
			return;
		}
	}

	VdAbstractFile* new_file = new VdFile(m_file_name, NORMALFILE, m_file_size);
	current_dir->AddAbstractFile(new_file);
}
