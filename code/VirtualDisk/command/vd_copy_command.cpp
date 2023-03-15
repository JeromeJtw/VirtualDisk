#include "../file/vd_abstract_file.h"
#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../file/vd_link_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_copy_command.h"
#include <algorithm>
#include <fstream>
#include <iostream>

VdCopyCommand::VdCopyCommand()
{

}

VdCopyCommand::~VdCopyCommand()
{

}

void VdCopyCommand::ClearParameter()
{
	m_scr_file_string = "";
	m_dst_file_string = "";
	m_src_path.clear();
	m_dst_path.clear();
	m_scr_file = nullptr;
	m_dst_file = nullptr;

	m_dst_dir = nullptr;
	m_dst_file_name = "";

}

bool VdCopyCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	if (command_para.size() < 3)
	{
		return false;
	}
	m_scr_file_string = command_para[1];
	m_dst_file_string = command_para[2];
	return true;
}

void VdCopyCommand::Execute(VdSystemLogic* vd_system)
{
	if (vd_system == nullptr)
	{
		return;
	}
	PreExecute();
	if (!ParseParameter(vd_system))
	{
		std::cout << "�����﷨�������������룡" << std::endl;
		return;
	}
	if (strcmp(m_scr_file_string.c_str(), m_dst_file_string.c_str()) == 0)
	{
		std::cout << "�޷����������ƣ�" << std::endl;
		return;
	}
	Copy(vd_system);
	PostExecute(vd_system);
}

void VdCopyCommand::PrintHelp()
{
	std::cout << "��һ�ݻ����ļ����Ƶ���һ��λ�á�" << std::endl;
}

void VdCopyCommand::Copy(VdSystemLogic* vd_system)
{
	//������������·������������̿���
	if ((*m_scr_file_string.begin()) == '@')
	{
		CopyFromPhysicalDisk(vd_system);
		return;
	}
	
	std::replace(m_scr_file_string.begin(), m_scr_file_string.end(), '/', '\\');
	m_src_path= VdTool::SplitString(m_scr_file_string, "\\");

	std::replace(m_dst_file_string.begin(), m_dst_file_string.end(), '/', '\\');
	m_dst_path = VdTool::SplitString(m_dst_file_string, "\\");

	m_scr_file = vd_system->GetFileByPath(m_src_path);
	m_dst_file = vd_system->GetFileByPath(m_dst_path);
	if (m_scr_file == nullptr)
	{
		std::cout << "ϵͳ�Ҳ���ָ�����ļ���" << std::endl;
		return;
	}
	
	if (m_dst_file != nullptr && m_dst_file->GetAbstractFileType() != DIR)
	{
		std::cout << "Ŀ���ļ��Ѵ��ڣ��޷����и��ƣ�" << std::endl;
		return;
	}

	m_dst_file_name = (*(m_dst_path.end() - 1));
	if (m_dst_path.size() == 1)
	{
		m_dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	}
	else
	{
		std::vector<std::string> dst_dir_path(m_dst_path.begin(), m_dst_path.end() - 1);
		m_dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetFileByPath(dst_dir_path));
	}

	if (m_scr_file->GetAbstractFileType() == DIR && 
		m_dst_file != nullptr &&
		m_dst_file->GetAbstractFileType() == DIR)
	{
		CopyDirToDir(vd_system);
	}

	if (m_dst_dir == nullptr)
	{
		std::cout << "ϵͳ�Ҳ���ָ����·����" << std::endl;
		return;
	}

	if (m_scr_file->GetAbstractFileType() == NORMALFILE)
	{
		CopyNormalFile(vd_system);
		return;
	}

	if (m_scr_file->GetAbstractFileType() == LINKFILE)
	{
		CopyLinkFile(vd_system);
		return;
	}

	if (m_scr_file->GetAbstractFileType() == DIR)
	{
		CopyDir(vd_system);
		return;
	}
}

void VdCopyCommand::CopyFromPhysicalDisk(VdSystemLogic* vd_system)
{
	m_scr_file_string.erase(m_scr_file_string.begin());
	
	std::ifstream src_file;
	src_file.open(m_scr_file_string, std::ios::in | std::ios::binary | std::ios::ate);
	if (!src_file.is_open())
	{
		std::cout << m_scr_file_string << " �ļ���ʧ�ܣ����顣" << std::endl;
		return;
	}

	std::replace(m_dst_file_string.begin(), m_dst_file_string.end(), '/', '\\');
	m_dst_path = VdTool::SplitString(m_dst_file_string, "\\");
	m_dst_file = vd_system->GetFileByPath(m_dst_path);

	if (m_dst_file != nullptr)
	{
		std::cout << "Ŀ���ļ��Ѵ��ڣ��޷����и��ƣ�" << std::endl;
		return;
	}

	m_dst_file_name = (*(m_dst_path.end() - 1));
	if (m_dst_path.size() == 1)
	{
		m_dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetCurrentFile());
	}
	else
	{
		std::vector<std::string> dst_dir_path(m_dst_path.begin(), m_dst_path.end() - 1);
		m_dst_dir = dynamic_cast<VdDirectory*>(vd_system->GetFileByPath(dst_dir_path));
	}

	if (m_dst_dir == nullptr)
	{
		std::cout << "ϵͳ�Ҳ���ָ����·����" << std::endl;
		return;
	}

	int file_size = (int)src_file.tellg();
	src_file.seekg(0, std::ios::beg);
	char* buffer = new char[file_size];
	src_file.read(buffer, file_size);
	src_file.close();

	VdFile* new_file = new VdFile(m_dst_file_name, NORMALFILE, file_size, buffer);
	delete buffer;
	m_dst_dir->AddAbstractFile(new_file);
	std::cout << "���Ƴɹ�!" << std::endl;
}

void VdCopyCommand::CopyNormalFile(VdSystemLogic* vd_system)
{
	VdFile *src_file = dynamic_cast<VdFile*>(m_scr_file);
	VdFile *new_file = new VdFile(*src_file);
	new_file->ReName(m_dst_file_name);
	m_dst_dir->AddAbstractFile(new_file);
}

void VdCopyCommand::CopyLinkFile(VdSystemLogic* vd_system)
{
	VdLinkFile *src_file = dynamic_cast<VdLinkFile*>(m_scr_file);
	VdLinkFile *new_file = new VdLinkFile(*src_file);
	new_file->ReName(m_dst_file_name);
	m_dst_dir->AddAbstractFile(new_file);
}

void VdCopyCommand::CopyDir(VdSystemLogic* vd_system)
{
	VdDirectory* src_dir = dynamic_cast<VdDirectory*>(m_scr_file);
	std::vector<VdAbstractFile*> normal_file_list = src_dir->GetNormalSubFileList();
	int file_size = 0;
	for (auto iter : normal_file_list)
	{
		if (iter->GetAbstractFileType() == NORMALFILE)
		{
			VdFile* file = dynamic_cast<VdFile*>(iter);
			file_size += file->GetFileSize();
		}
	}
	VdFile* new_file = new VdFile(m_dst_file_name, NORMALFILE, file_size);
	m_dst_dir->AddAbstractFile(new_file);
}

void VdCopyCommand::CopyDirToDir(VdSystemLogic* vd_system)
{
	VdDirectory* src_dir = dynamic_cast<VdDirectory*>(m_scr_file);
	VdDirectory* dst_dir = dynamic_cast<VdDirectory*>(m_dst_file);
	std::vector<VdAbstractFile*> src_dir_file = src_dir->GetNormalSubFileList();
	for (auto file : src_dir_file)
	{
		VdAbstractFile* new_file = nullptr;
		if (file->GetAbstractFileType() == NORMALFILE)
		{
			new_file = new VdFile(*dynamic_cast<VdFile*>(file));
		}
		else if (file->GetAbstractFileType() == LINKFILE)
		{
			new_file = new VdLinkFile(*dynamic_cast<VdLinkFile*>(file));
		}
		if (dst_dir->IsExistFile(file->GetAbstractFileName()))
		{
			dst_dir->DeleteSubFileByName(file->GetAbstractFileName());
		}
		dst_dir->AddAbstractFile(new_file);
		std::cout << "�����ļ� - " << file->GetParentPath() << " ��" << dst_dir->GetCurrentPath() << std::endl;
	}
}
