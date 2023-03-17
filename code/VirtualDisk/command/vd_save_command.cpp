#include "../file/vd_abstract_file.h"
#include "../file/vd_directory.h"
#include "../file/vd_file.h"
#include "../file/vd_link_file.h"
#include "../logic/vd_system_logic.h"
#include "../tool/vd_tool.h"
#include "vd_save_command.h"
#include <algorithm>
#include <iostream>
#include <time.h>

VdSaveCommand::VdSaveCommand()
{

}

VdSaveCommand::~VdSaveCommand()
{

}

void VdSaveCommand::ClearParameter()
{
	m_dst_file = "";
	m_file.close();
}

bool VdSaveCommand::ParseParameter(VdSystemLogic* vd_system)
{
	std::vector<std::string> command_para = vd_system->GetCommandPara();
	if (command_para.size() < 2)
	{
		return false;
	}
	m_dst_file = command_para[1];
	std::replace(m_dst_file.begin(), m_dst_file.end(), '\\', '/');
	return true;
}

void VdSaveCommand::Execute(VdSystemLogic* vd_system)
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
	Save(vd_system);
	PostExecute(vd_system);
}

void VdSaveCommand::PrintHelp()
{
	std::cout << "将虚拟磁盘序列化为文件。" << std::endl;
}

void VdSaveCommand::Save(VdSystemLogic* vd_system)
{
	VdAbstractFile* root = vd_system->GetDiskRoot();
	if (root == nullptr)
	{
		return;
	}

	size_t n = m_dst_file.find_last_of('/');
	std::string file_name = m_dst_file.substr(n + 1);
	std::string file_path = m_dst_file.substr(0, strlen(m_dst_file.c_str()) - strlen(file_name.c_str()) - 1);
	if (!VdTool::CreateDirectory(file_path))
	{
		std::cout << "无法创建文件，序列化失败！" << std::endl;
		return;
	}
	
	m_file.open(m_dst_file, std::ios::out);
	if (!m_file.is_open())
	{
		std::cout << "无法序列化，请检查！" << std::endl;
		return;
	}

	struct tm stime;
	time_t now = time(0);
	localtime_s(&stime, &now);
	char tmp[32] = { NULL };
	strftime(tmp, sizeof(tmp), "%Y/%m/%d  %H:%M:%S", &stime);
	m_file << "ViskDiskFile" << std::endl;
	m_file << "时间：" << tmp << std::endl;
	m_file << "姓名: jiantingwu" << std::endl;
	m_file << "=================================" << std::endl;
	VdDirectory* work_dir = dynamic_cast<VdDirectory*>(root);
	work_dir = dynamic_cast<VdDirectory*>(work_dir->GetNormalSubFileList()[0]);

	m_file << work_dir->GetAbstractFileType() << " " << work_dir->GetAbstractFileName() << std::endl;
	std::vector<VdAbstractFile*> file_list = work_dir->GetNormalSubFileList();
	std::string prefix_string = "*";
	for (auto file_iter : file_list)
	{
		SaveFile(file_iter, prefix_string);
		if (file_iter->GetAbstractFileType() == DIR)
		{
			VdDirectory* dir = dynamic_cast<VdDirectory*>(file_iter);
			SaveDir(dir, prefix_string);
		}
	}
	m_file.close();
	std::cout << "序列化成功!" << std::endl;
	return;
}

void VdSaveCommand::SaveDir(VdDirectory* dir, std::string prefix)
{
	prefix += "*";
	std::vector<VdAbstractFile*> sub_file_list = dir->GetNormalSubFileList();
	for (auto iter : sub_file_list)
	{
		SaveFile(iter, prefix);
		if (iter->GetAbstractFileType() == DIR)
		{
			VdDirectory* vd_dir = dynamic_cast<VdDirectory*>(iter);
			SaveDir(vd_dir, prefix);
		}
	}
}

void VdSaveCommand::SaveFile(VdAbstractFile* save_file, std::string prefix)
{
	if (save_file->GetAbstractFileType() == NORMALFILE)
	{
		VdFile* file = dynamic_cast<VdFile*>(save_file);
		std::string file_name = "\"" + file->GetAbstractFileName() + "\"";
		std::string file_content(file->GetFileContent(), file->GetFileSize());
		std::string save_file_content = "\"" + file_content + "\"";
		m_file << prefix << file->GetAbstractFileType() << " " << file_name << " " << file->GetFileSize()
			<< " " << save_file_content << std::endl;
	}
	else if (save_file->GetAbstractFileType() == LINKFILE)
	{
		VdLinkFile* link_file = dynamic_cast<VdLinkFile*>(save_file);
		std::string file_name = "\"" + link_file->GetAbstractFileName() + "\"";
		if (link_file->GetLinkFile())
		{
			std::string link_file_path = "\"" + link_file->GetLinkFile()->GetCurrentPath() + "\"";
			m_file << prefix << link_file->GetAbstractFileType() << " " << file_name << " " <<
				link_file_path << std::endl;
		}
		else
		{
			m_file << prefix << link_file->GetAbstractFileType() << " " << file_name << " " << std::endl;
		}
	}
	else
	{
		m_file << prefix << save_file->GetAbstractFileType() << " " << save_file->GetAbstractFileName() << std::endl;
	}
}

