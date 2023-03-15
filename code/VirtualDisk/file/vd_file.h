/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_file.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_abstract_file.h"

class VdFile : public VdAbstractFile
{

public:
	VdFile(std::string name, VdFileType file_type, int file_size);
	VdFile(std::string name, VdFileType file_type, int file_size, const char* file_content);
	VdFile(const VdFile& file);
	virtual ~VdFile();

	void PrintPath() override;
	void PrintFileInfo() override;
	int GetFileSize() const { return m_file_size; }
	char* GetFileContent() const { return m_file_content; }
private:
	int m_file_size;
	char* m_file_content = nullptr;
};