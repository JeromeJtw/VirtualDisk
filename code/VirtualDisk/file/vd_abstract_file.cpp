#include "vd_abstract_file.h"
#include <time.h>
#include <iostream>
#include "../tool/vd_tool.h"

VdAbstractFile::VdAbstractFile(std::string name, int file_type)
	: m_name(name), m_vd_file_type(file_type)
{
	m_parent = nullptr;
	struct tm stime;
	time_t now = time(0);
	localtime_s(&stime, &now);
	char tmp[30] = { NULL };
	strftime(tmp, sizeof(tmp), "%Y/%m/%d  %H:%M", &stime);
	m_time = tmp;
}

VdAbstractFile::VdAbstractFile(const VdAbstractFile& file)
{

}


VdAbstractFile::~VdAbstractFile()
{

}

void VdAbstractFile::PrintPath()
{
	std::cout << m_name << "\\>";
}

void VdAbstractFile::ReName(const std::string& new_name)
{
	m_name = new_name;
}

int VdAbstractFile::AddAbstractFile(VdAbstractFile* vd_abstract_file)
{
	return ADDSUCCESSED;
}

void VdAbstractFile::PrintFileInfo()
{
	std::cout << m_time << "    " << "<DIR>    " << "          " << m_name << std::endl;
}

void VdAbstractFile::SetParent(VdAbstractFile* parent_file)
{
	m_parent = parent_file;
}

std::string VdAbstractFile::GetCurrentPath()
{
	std::string path = GetParentPath();
	path.erase(path.begin());
	return path;
}

std::string VdAbstractFile::GetParentPath()
{
	if (m_parent == nullptr)
	{
		return "";
	}
	return m_parent->GetParentPath() + "\\" + m_name;
}

std::string VdAbstractFile::GetSerializationFileName()
{
	std::string name = GetParentSerializationFileName();
	return name;
}

std::string VdAbstractFile::GetParentSerializationFileName()
{
	if (m_parent == nullptr)
	{
		return "";
	}
	if (m_parent->GetAbstractFileName() == ROOTDISKNAME)
	{
		return CDISKNAME + m_name;
	}
	return m_parent->GetParentSerializationFileName() + m_name;
}
