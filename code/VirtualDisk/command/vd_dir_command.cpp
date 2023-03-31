#include "vd_dir_command.h"
#include "../file/vd_abstract_file.h"
#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include <algorithm>
#include <iostream>
static const std::string DIRRECURSIONFLAGSTR = "/s";
static const std::string ONLYPRINTDIRFLAG = "/ad";

VdDirCommand::VdDirCommand()
{

}

VdDirCommand::~VdDirCommand()
{
	ClearParameter();
}

void VdDirCommand::ClearParameter()
{
	m_recursion_print = false;
	m_print_dir = false;
	m_dst_dir = "";
}

bool VdDirCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	std::vector<std::string>::iterator recursion_iter = std::find(command_para.begin(), command_para.end(), DIRRECURSIONFLAGSTR);
	if (recursion_iter != command_para.end())
	{
		m_recursion_print = true;
		command_para.erase(recursion_iter);
	}

	std::vector<std::string>::iterator iter = std::find(command_para.begin(), command_para.end(), ONLYPRINTDIRFLAG);
	if (iter != command_para.end())
	{
		m_print_dir = true;
		command_para.erase(iter);
	}

	command_para.erase(command_para.begin());
	if (command_para.size() > 0)
	{
		m_dst_dir = command_para[0];
	}
	return true;
}

void VdDirCommand::Execute(VdSystemLogic* vd_system)
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
	if (m_dst_dir.empty())
	{
		PrintCurrentDir(vd_system);
	}
	else
	{
		PrintDir(vd_system, m_dst_dir);
	}
	PostExecute(vd_system);
}

void VdDirCommand::PrintHelp()
{
	std::cout << "��ʾĿ¼�е��ļ�����Ŀ¼�б�" << std::endl;
}

void VdDirCommand::PrintCurrentDir(VdSystemLogic* vd_system)
{
	if (m_recursion_print)
	{
		RecursionPrint(vd_system);
	}
	else
	{
		NormalPrint(vd_system);
	}
}

void VdDirCommand::NormalPrint(VdSystemLogic* vd_system)
{
	VdAbstractFile* current_file = vd_system->GetActiveFile();
	if (current_file == nullptr || current_file->GetAbstractFileType() != DIR)
		return;

	VdDirectory* dir = dynamic_cast<VdDirectory*>(current_file);
	std::vector<VdAbstractFile*> sub_file_list = dir->GetSubFileList();
	int file_num = 0;
	int dir_num = 0;

	std::string current_dir = dir->IsRootDir() ? current_file->GetCurrentPath() + "\\ ��Ŀ¼" : current_file->GetCurrentPath() + " ��Ŀ¼";
	std::cout << current_dir << std::endl;
	for (auto file : sub_file_list)
	{
		if (m_print_dir && file->GetAbstractFileType() != DIR)
		{
			continue;
		}
		file->PrintFileInfo();
		switch (file->GetAbstractFileType())
		{
		case DIR:
			dir_num++;
			break;
		case NORMALFILE:
		case LINKFILE:
			file_num++;
			break;
		default:
			break;
		}
	}
	std::cout << "                               " << file_num << "���ļ�" << std::endl;
	std::cout << "                               " << dir_num << "��Ŀ¼" << std::endl;
}

void VdDirCommand::RecursionPrint(VdSystemLogic* vd_system)
{
	VdAbstractFile* current_file = vd_system->GetActiveFile();
	if (current_file && current_file->GetAbstractFileType() == DIR)
	{
		VdDirectory* dir = dynamic_cast<VdDirectory*>(current_file);
		int file_count = 0, dir_count = 0;
		dir->RecursionPrintFileInfo(m_print_dir, file_count, dir_count);
		std::cout << std::endl << "                  �����ļ�����: " << std::endl;
		std::cout << "                          " << file_count << "���ļ�" << std::endl;
		std::cout << "                          " << dir_count << "��Ŀ¼" << std::endl;
	}
}

void VdDirCommand::PrintDir(VdSystemLogic* vd_system, std::string& dir)
{
	std::replace(dir.begin(), dir.end(), '/', '\\');
	std::vector<std::string> dir_path = VdTool::SplitString(dir, "\\");
	for (auto iter : dir_path)
	{
		if (!vd_system->ChangeActiveDir(iter))
		{
			return;
		}
	}
	PrintCurrentDir(vd_system);
}
