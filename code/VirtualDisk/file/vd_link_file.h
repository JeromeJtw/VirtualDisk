/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_link_file.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_abstract_file.h"

class VdLinkFile : public VdAbstractFile
{

public:
	VdLinkFile(std::string name, VdFileType file_type);
	VdLinkFile(const VdLinkFile& file);
	virtual ~VdLinkFile();

	void PrintPath() override;
	void PrintFileInfo() override;
	void SetLinkFile(VdAbstractFile* link_file);
	VdAbstractFile* GetLinkFile() const { return m_link_file; }
	void SetLinkFilePath(const std::string file_path);
	std::string GetLinkFilePath() const { return m_link_file_path; }
private:
	VdAbstractFile* m_link_file;
	std::string		m_link_file_path;
};
