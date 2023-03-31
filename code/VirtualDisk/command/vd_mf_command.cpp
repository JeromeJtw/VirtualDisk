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

	int res = VdTool::IsVaildFileName(m_file_name);
	if (res == TOOLONG)
	{
		std::cout << "目标文件名超过" << MAX_NAME_LENGTH << "个字符，无法创建。" << std::endl;
		return;
	}
	if (res == HASINVAILDCHAR)
	{
		std::cout << "文件名包含非法字符，无法创建。" << std::endl;
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
	std::cout << "文件"<<m_file_name << "创建成功" << std::endl;
}
