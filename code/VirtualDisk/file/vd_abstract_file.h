/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_abstract_file.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "../vd_enum.h"
#include <string>

class VdAbstractFile
{

public:
	//VdAbstractFile();
	VdAbstractFile(std::string name, VdFileType file_type);
	VdAbstractFile(const VdAbstractFile& file);
	virtual ~VdAbstractFile();


	virtual void PrintPath();
	virtual void ReName(const std::string new_name);
	virtual	bool AddAbstractFile(VdAbstractFile* vd_abstract_file);
	virtual void PrintFileInfo();

	void SetParent(VdAbstractFile* parent_file);
	VdAbstractFile* GetParent() const { return m_parent; }
	std::string GetCurrentPath();
	std::string GetParentPath();
	int GetAbstractFileType() const { return m_vd_file_type; }
	std::string GetAbstractFileName() const { return m_name; }
protected:
	std::string m_name;
	VdFileType m_vd_file_type;
	std::string m_time;
	VdAbstractFile* m_parent;
};
