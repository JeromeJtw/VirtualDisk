#include "vd_directory.h"
#include "vd_link_file.h"
#include <iostream>
#include <regex>

VdDirectory::VdDirectory(std::string name, VdFileType file_type)
			:VdAbstractFile(name, file_type)
{
	AddDefaultDir();
}

VdDirectory::~VdDirectory()
{
	m_file_list.clear();
}

void VdDirectory::PrintPath()
{
	VdAbstractFile::PrintPath();
}

bool VdDirectory::AddAbstractFile(VdAbstractFile* vd_abstract_file)
{
	if (IsExistFile(vd_abstract_file->GetAbstractFileName()))
	{
		return false;
	}
	vd_abstract_file->SetParent(this);
	m_file_list.emplace_back(vd_abstract_file);
	return true;
}


void VdDirectory::AddDefaultDir()
{
	VdLinkFile* dir1 = new VdLinkFile(".", DIR);
	dir1->SetLinkFile(this);
	VdLinkFile* dir2 = new VdLinkFile("..", DIR);
	dir2->SetLinkFile(this->GetParent());
	AddAbstractFile(dir1);
	AddAbstractFile(dir2);
}

int VdDirectory::RemoveSubDirByName(const std::string name)
{
	if (name == "." || name == "..")
	{
		return CANNOTRM;
	}
	bool is_find = false;
	for (auto iter = m_file_list.begin(); iter != m_file_list.end(); iter++)
	{
		if ((*iter)->GetAbstractFileName() == name)
		{
			is_find = true;
			if ((*iter)->GetAbstractFileType() == NORMALFILE)
			{
				return CANNOTRM;
			}

			if ((*iter)->GetAbstractFileType() == DIR)
			{
				VdDirectory* dir = dynamic_cast<VdDirectory*>(*iter);
				if (dir->GetNormalSubFileList().size() == 0) 
				{
					delete (*iter);
					m_file_list.erase(iter);
					return SUCCESSED;
				}
				return NOTEMPTY;
			}
		}
	}
	if (!is_find)
	{
		return NOTFIND;
	}
	return SUCCESSED;
}

void VdDirectory::RemoveSubDir(VdAbstractFile* dir)
{
	std::vector<VdAbstractFile*>::iterator iter  = m_file_list.begin();
	for (iter; iter != m_file_list.end(); iter++)
	{
		if ((*iter)->GetAbstractFileName() == dir->GetAbstractFileName() && (*iter)->GetAbstractFileType() == DIR)
		{
			delete *iter;
			m_file_list.erase(iter);
			break;
		}
	}
}

void VdDirectory::EraseSubFileByName(const std::string name)
{
	if (name == "." || name == "..")
	{
		return;
	}
	std::vector<VdAbstractFile*>::iterator iter = m_file_list.begin();
	for (auto iter = m_file_list.begin(); iter != m_file_list.end(); iter++)
	{
		if ((*iter)->GetAbstractFileName() == name)
		{
			m_file_list.erase(iter);
			return;
		}
	}
}


int VdDirectory::DeleteSubFileByName(const std::string name)
{
	if (name == "." || name == "..")
	{
		return CANNOTRM;
	}
	bool is_find = false;
	for (auto iter = m_file_list.begin(); iter != m_file_list.end(); iter++)
	{
		if ((*iter)->GetAbstractFileName() == name)
		{
			is_find = true;
			if ((*iter)->GetAbstractFileType() == NORMALFILE 
				|| (*iter)->GetAbstractFileType() == LINKFILE)
			{
				delete (*iter);
				m_file_list.erase(iter);
				return SUCCESSED;
			}

			if ((*iter)->GetAbstractFileType() == DIR)
			{
				return CANNOTRM;
			}
		}
	}
	if (!is_find)
	{
		return NOTFIND;
	}
	return SUCCESSED;
}

void VdDirectory::DeleteAllNormalFile()
{
	for (auto iter = m_file_list.begin(); iter != m_file_list.end();)
	{
		if ((*iter)->GetAbstractFileType() == NORMALFILE ||
			(*iter)->GetAbstractFileType() == LINKFILE)
		{
			delete *iter;
			*iter = nullptr;
			iter = m_file_list.erase(iter);
			continue;
		}
		iter++;
	}
}

void VdDirectory::RecursionDeleteSubFileByName(const std::string name, const bool is_print_info /*= true*/)
{
	if (m_file_list.size() == 0)
	{
		return;
	}
	for (auto iter = m_file_list.begin() + 2; iter != m_file_list.end();)
	{
		if ((*iter)->GetAbstractFileType() == DIR)
		{
			VdDirectory* dir = dynamic_cast<VdDirectory*>(*iter);
			dir->RecursionDeleteSubFileByName(name);
		}
		if ((*iter)->GetAbstractFileName() == name)
		{
			if (is_print_info)
			{
				std::cout << "删除文件 - " << (*iter)->GetCurrentPath() << std::endl;
			}
			delete *iter;
			*iter = nullptr;
			iter = m_file_list.erase(iter);
			continue;
		}
		iter++;
	}
}

void VdDirectory::RecursionDeleteAllFile()
{
	for (auto iter = m_file_list.begin(); iter != m_file_list.end(); )
	{
		VdDirectory* dir = dynamic_cast<VdDirectory*>(*iter);
		if (dir)
		{
			dir->RecursionDeleteAllFile();
			iter++;
		}
		else
		{
			if ((*iter)->GetAbstractFileName() == "." || (*iter)->GetAbstractFileName() == "..")
			{
				iter++;
				continue;
			}
			delete *iter;
			*iter = nullptr;
			iter = m_file_list.erase(iter);
		}
	}
}

void VdDirectory::RecursionDestoryDir()
{
	for (auto iter = m_file_list.begin(); iter != m_file_list.end(); )
	{
		VdDirectory* dir = dynamic_cast<VdDirectory*>(*iter);
		if (dir)
		{
			dir->RecursionDestoryDir();
			delete dir;
			dir = nullptr;
			iter = m_file_list.erase(iter);
		}
		else
		{
			delete *iter;
			*iter = nullptr;
			iter = m_file_list.erase(iter);
		}
	}
}

std::vector<VdAbstractFile*> VdDirectory::GetNormalSubFileList() const
{
	std::vector<VdAbstractFile*> normal_file_list;
	if (m_file_list.size() > 2)
	{
		normal_file_list.insert(normal_file_list.end(), m_file_list.begin() + 2, m_file_list.end());
	}
	return normal_file_list;
}

std::vector<VdAbstractFile*> VdDirectory::GetNormalSubFileByVagueName(const std::string vague_name) const
{
	std::vector<VdAbstractFile*> file_list;
	std::regex regex(vague_name);
	for (auto iter : m_file_list)
	{
		if (std::regex_match(iter->GetAbstractFileName(), regex))
		{
			file_list.emplace_back(iter);
		}
	}
	return file_list;
}

std::vector<VdDirectory*> VdDirectory::GetDir() const
{
	std::vector<VdDirectory*> dir_list;
	for (auto iter : m_file_list)
	{
		if (iter->GetAbstractFileType() == DIR && 
			iter->GetAbstractFileName() != "." && 
			iter->GetAbstractFileName() != "..")
		{
			VdDirectory* dir = dynamic_cast<VdDirectory*>(iter);
			dir_list.emplace_back(dir);
		}
	}
	return dir_list;
}

VdAbstractFile* VdDirectory::GetSubFileByName(const std::string name)
{
	for (auto iter = m_file_list.begin(); iter != m_file_list.end();iter++)
	{
		if ((*iter)->GetAbstractFileName() == name)
		{
			return (*iter);
		}
	}
	return nullptr;
}

void VdDirectory::RecursionPrintFileInfo(const bool is_only_print_dir, int& file_count, int& dir_count)
{
	std::string dir_path = this->IsRootDir() ? this->GetCurrentPath() + "\\" : this->GetCurrentPath();
	std::cout << std::endl << dir_path << " 的目录" << std::endl;
	int file_num = 0;
	int dir_num = 0;
	for (auto iter : m_file_list)
	{
		if (is_only_print_dir && iter->GetAbstractFileType() != DIR)
		{
			continue;
		}
		iter->PrintFileInfo();
		switch (iter->GetAbstractFileType())
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

	for (auto iter : m_file_list)
	{
		if (iter->GetAbstractFileType() == DIR)
		{
			VdDirectory* dir = dynamic_cast<VdDirectory*>(iter);
			if (dir)
			{
				dir->RecursionPrintFileInfo(is_only_print_dir, file_count, dir_count);
			}
		}
	}
	file_count += file_num;
	dir_count += dir_num;
}

bool VdDirectory::IsExistFile(const std::string name)
{
	for (auto iter : m_file_list)
	{
		if (iter->GetAbstractFileName() == name)
		{
			return true;
		}
	}
	return false;
}

