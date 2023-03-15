#include "../tool/vd_tool.h"
#include "vd_link_file.h"
#include <iostream>

extern std::unordered_map<int, std::string> FILE_TYPE_STRING;

VdLinkFile::VdLinkFile(std::string name, VdFileType file_type)
	:VdAbstractFile(name, file_type)
{

}

VdLinkFile::VdLinkFile(const VdLinkFile& file)
	:VdAbstractFile(file.GetAbstractFileName(),LINKFILE)
{
	m_link_file_path = file.GetLinkFilePath();
	m_link_file = file.GetLinkFile();
}

VdLinkFile::~VdLinkFile()
{
	m_link_file = nullptr;
	m_link_file_path = "";
}

void VdLinkFile::PrintPath()
{

}

void VdLinkFile::PrintFileInfo()
{
	if (GetAbstractFileType() == DIR)
	{
		std::cout << m_time << "    " << FILE_TYPE_STRING[this->GetAbstractFileType()] << "              " << m_name << std::endl;
		return;
	}
	std::cout << m_time << "    " << FILE_TYPE_STRING[this->GetAbstractFileType()] << "          " << m_name << std::endl;
}

void VdLinkFile::SetLinkFile(VdAbstractFile* link_file)
{
	m_link_file = link_file;
	if (m_link_file)
	{
		m_link_file_path = m_link_file->GetCurrentPath();
	}
}

void VdLinkFile::SetLinkFilePath(const std::string file_path)
{
	m_link_file_path = file_path;
}
