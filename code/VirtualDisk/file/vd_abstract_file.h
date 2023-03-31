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
	VdAbstractFile(std::string name, int file_type);
	VdAbstractFile(const VdAbstractFile& file);
	virtual ~VdAbstractFile();


	virtual void PrintPath();
	virtual void ReName(const std::string& new_name);
	virtual	int AddAbstractFile(VdAbstractFile* vd_abstract_file);
	virtual void PrintFileInfo();

	void SetParent(VdAbstractFile* parent_file);
	VdAbstractFile* GetParent() const { return m_parent; }
	std::string GetCurrentPath();
	std::string GetParentPath();
	int GetAbstractFileType() const { return m_vd_file_type; }
	std::string GetAbstractFileName() const { return m_name; }
	std::string GetSerializationFileName();
	void SetRootFlag(bool root = false) { m_is_root_dir = root; }
	bool IsRootDir() const { return m_is_root_dir; }

private:
	std::string GetParentSerializationFileName();

protected:
	std::string			m_name;
	int					m_vd_file_type;
	std::string			m_time;
	VdAbstractFile*		m_parent;
	bool				m_is_root_dir = false;
};
