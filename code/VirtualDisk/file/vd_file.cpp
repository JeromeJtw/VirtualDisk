#include "vd_file.h"
#include <iostream>

VdFile::VdFile(std::string name, VdFileType file_type, int file_size)
	:VdAbstractFile(name,file_type)
{
	m_file_size = file_size;
	m_file_content = (char*)malloc(m_file_size);
	memset(m_file_content, 0, m_file_size);
}

VdFile::VdFile(const VdFile& file)
	:VdAbstractFile(file.m_name,(VdFileType)file.m_vd_file_type)
{
	this->m_file_size = file.m_file_size;
	this->m_file_content = (char*)malloc(this->m_file_size);
	memcpy(this->m_file_content, file.m_file_content, this->m_file_size);
}

VdFile::VdFile(std::string name, VdFileType file_type, int file_size, const char* file_content):
	VdAbstractFile(name, file_type)
{
	m_file_size = file_size;
	m_file_content = (char*)malloc(m_file_size);
	memset(m_file_content, 0, m_file_size);
	memcpy(this->m_file_content, file_content, this->m_file_size);
}

VdFile::~VdFile()
{
	if (m_file_content)
	{
		free(m_file_content);
	}
	m_file_content = nullptr;
}

void VdFile::PrintPath()
{

}

void VdFile::PrintFileInfo()
{
	std::cout << m_time << "    " << "       " << m_file_size << "         " << m_name << std::endl;
}
