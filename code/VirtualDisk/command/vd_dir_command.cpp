#include "vd_dir_command.h"
#include "../file/vd_abstract_file.h"
#include "../file/vd_directory.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include <algorithm>
#include <iostream>

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
	std::vector<std::string>::iterator recursion_iter = std::find(command_para.begin(), command_para.end(), "/s");
	if (recursion_iter != command_para.end())
	{
		m_recursion_print = true;
		command_para.erase(recursion_iter);
	}

	std::vector<std::string>::iterator iter = std::find(command_para.begin(), command_para.end(), "/ad");
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
		std::cout << "命令语法不正确，请重新输入！" << std::endl;
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
	std::cout << "显示目录中的文件和子目录列表。" << std::endl;
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
	if (current_file && current_file->GetAbstractFileType() == DIR)
	{
		VdDirectory* dir = dynamic_cast<VdDirectory*>(current_file);
		std::vector<VdAbstractFile*> sub_file_list = dir->GetSubFileList();
		int abstract_file_num = (int)sub_file_list.size();
		int file_num = 0;
		int dir_num = 0;

		std::string current_dir = dir->IsRootDir() ? current_file->GetCurrentPath() + "\\ 的目录" : current_file->GetCurrentPath() + " 的目录";
		std::cout << current_dir << std::endl;
		for (int i = 0; i < abstract_file_num; i++)
		{
			if (m_print_dir && sub_file_list[i]->GetAbstractFileType() != DIR)
			{
				continue;
			}
			sub_file_list[i]->PrintFileInfo();
			switch (sub_file_list[i]->GetAbstractFileType())
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
		std::cout << "                               " << file_num << "个文件" << std::endl;
		std::cout << "                               " << dir_num << "个目录" << std::endl;
	}
}

void VdDirCommand::RecursionPrint(VdSystemLogic* vd_system)
{
	VdAbstractFile* current_file = vd_system->GetActiveFile();
	if (current_file && current_file->GetAbstractFileType() == DIR)
	{
		VdDirectory* dir = dynamic_cast<VdDirectory*>(current_file);
		int file_count = 0, dir_count = 0;
		dir->RecursionPrintFileInfo(m_print_dir, file_count, dir_count);
		std::cout << std::endl << "                  所列文件总数: " << std::endl;
		std::cout << "                          " << file_count << "个文件" << std::endl;
		std::cout << "                          " << dir_count << "个目录" << std::endl;
	}
}

void VdDirCommand::PrintDir(VdSystemLogic* vd_system, std::string dir)
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
