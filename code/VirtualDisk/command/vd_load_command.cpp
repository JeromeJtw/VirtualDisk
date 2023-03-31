#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../file/vd_link_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_load_command.h"
#include <algorithm>
#include <iostream>
#include <fstream>

VdLoadCommand::VdLoadCommand()
{

}

VdLoadCommand::~VdLoadCommand()
{

}

void VdLoadCommand::ClearParameter()
{
	m_scr_file = "";
	m_scr_path = "";
}

bool VdLoadCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	if (command_para.size() < 2)
	{
		return false;
	}
	m_scr_file = command_para[1];
	return true;
}

void VdLoadCommand::Execute(VdSystemLogic* vd_system)
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
	LoadFromFile(vd_system);
	PostExecute(vd_system);
}


void VdLoadCommand::PrintHelp()
{
	std::cout << "将文件反序列化为虚拟磁盘。" << std::endl;
}

void VdLoadCommand::LoadFromFile(VdSystemLogic* vd_system)
{
	std::ifstream src_file;
	std::replace(m_scr_file.begin(), m_scr_file.end(), '\\', '/');
	m_scr_path = m_scr_file;
	size_t flag = m_scr_file.find_last_of('/');
	std::string src_file_name = m_scr_file.substr(flag + 1);
	m_scr_file += "/" + src_file_name + SERIALIZATIONFILESUFFIX;
	src_file.open(m_scr_file);
	if (!src_file.is_open())
	{
		std::cout << "无法打开文件，请检查！" << std::endl;
		return;
	}

	VdDirectory* root = dynamic_cast<VdDirectory*>(vd_system->GetDiskRoot());
	std::vector<VdAbstractFile*> root_dir_list = root->GetNormalSubFileList();
	VdDirectory* disk_c = dynamic_cast<VdDirectory*>(root_dir_list[0]);
	if (disk_c == nullptr)
	{
		std::cout << "无法进行反序列化！" << std::endl;
		return;
	}

	int index = 1;
	int pre_prefix_count = 1;
	VdDirectory* parent_dir = disk_c;

	std::string line_content;
	while (getline(src_file,line_content))
	{
		if (index == 1 && strcmp(line_content.c_str(), "ViskDiskFile") != 0)
		{
			std::cout << "文件为非法的虚拟磁盘文件，无法进行反序列化！" << std::endl;
			return;
		}
		if (index == 5 && strcmp(line_content.c_str(), "1 C:") != 0)
		{
			std::cout << "文件为非法的虚拟磁盘文件，无法进行反序列化！" << std::endl;
			return;
		}
		if (index <= 5)
		{
			index++;
			continue;
		}
		int prefix_index = (int)line_content.find_last_of(SERIALIZATIONPREFIX);
		if (prefix_index == -1)
		{
			index++;
			continue;
		}
		std::string file_type_string = line_content.substr(prefix_index + 1, 1);
		std::string file_info = line_content.substr(prefix_index + 3);
		int file_type = atoi(file_type_string.c_str());

		if (prefix_index + 1 == pre_prefix_count)
		{
			pre_prefix_count = prefix_index + 1;
			parent_dir = parent_dir;
		}
		else if ((prefix_index == pre_prefix_count))
		{
			pre_prefix_count = prefix_index + 1;
			parent_dir = m_upper_level_dir;
		}
		else
		{
			int n = pre_prefix_count - (prefix_index + 1);
			while (n > 0)
			{
				parent_dir = dynamic_cast<VdDirectory*>(parent_dir->GetParent());
				n--;
			}
			pre_prefix_count = prefix_index + 1;
		}

		switch (file_type)
		{
		case DIR:
			CreateDir(file_info, parent_dir);
			break;
		case NORMALFILE:
			CreateNormalFile(file_info, parent_dir);
			break;
		case LINKFILE:
			CreateLinkFile(file_info, parent_dir);
			break;
		default:
			break;
		}
		index++;
	}
	src_file.close();
	PostHandleLinkFile(vd_system, disk_c);
	std::cout << "反序列化成功！" << std::endl;
}

void VdLoadCommand::CreateNormalFile(const std::string& file_info, VdDirectory* parent)
{
	std::vector<std::string> infos = VdTool::SplitStringBySpace(file_info);
	VdTool::ClearQuote(infos);
	VdFile* file = nullptr;
	if (infos.size() < 2)
	{
		return;
	}
	if (infos.size() == 2)
	{
		file = new VdFile(infos[0], NORMALFILE, atoi(infos[1].c_str()));
	}
	else if (infos.size() == 3)
	{
		std::string file_name = m_scr_path + "/" + infos[2];
		std::ifstream normal_file;
		normal_file.open(file_name, std::ios::in | std::ios::binary | std::ios::ate);
		if (!normal_file.is_open())
		{
			std::cout << infos[0] << "无法反序列化，请检查！" << std::endl;
			return;
		}
		int file_size = atoi(infos[1].c_str());
		normal_file.seekg(0, std::ios::beg);
		char* buffer = new char[file_size];
		normal_file.read(buffer, file_size);
		normal_file.close();
		file = new VdFile(infos[0], NORMALFILE, file_size, buffer);
		delete buffer;
		buffer = nullptr;
	}
	if (parent->AddAbstractFile(file) == EXISTSAMENAMEFILE)
	{
		parent->DeleteSubFileByName(infos[0]);
		parent->AddAbstractFile(file);
	}
}

void VdLoadCommand::CreateDir(const std::string& file_info, VdDirectory* parent)
{
	VdDirectory* dir = new VdDirectory(file_info, DIR);
	if (parent->AddAbstractFile(dir) == EXISTSAMENAMEFILE)
	{
		VdDirectory* temp_dir = dynamic_cast<VdDirectory*>(parent->GetSubFileByName(file_info));
		temp_dir->RecursionDestoryDir();
		parent->RemoveSubDirByName(file_info);
		parent->AddAbstractFile(dir);
	}
	m_upper_level_dir = dir;
}

void VdLoadCommand::CreateLinkFile(const std::string& file_info, VdDirectory* parent)
{
	std::vector<std::string> infos = VdTool::SplitStringBySpace(file_info);
	VdTool::ClearQuote(infos);
	VdLinkFile* link_file = new VdLinkFile(infos[0], LINKFILE);
	link_file->SetLinkFilePath(infos[1]);
	if (parent->AddAbstractFile(link_file) == EXISTSAMENAMEFILE)
	{
		parent->DeleteSubFileByName(infos[0]);
		parent->AddAbstractFile(link_file);
	}
}

void VdLoadCommand::PostHandleLinkFile(VdSystemLogic* vd_system, VdDirectory* dir)
{
	std::vector<VdAbstractFile*> sub_file_list = dir->GetNormalSubFileList();
	for (auto file : sub_file_list)
	{
		if (file->GetAbstractFileType() == LINKFILE)
		{
			VdLinkFile* link_file = dynamic_cast<VdLinkFile*>(file);
			vd_system->PostHandleLinkFile(link_file);
		}
		if (file->GetAbstractFileType() == DIR)
		{
			VdDirectory* file_dir = dynamic_cast<VdDirectory*>(file);
			PostHandleLinkFile(vd_system, file_dir);
		}
	}
}
