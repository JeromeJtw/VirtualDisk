/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_copy_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdCopyCommand : public VdCommand
{

public:
	VdCopyCommand();
	virtual ~VdCopyCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void Copy(VdSystemLogic* vd_system);
	void CopyFromPhysicalDisk(VdSystemLogic* vd_system);
	void CopyNormalFile(VdSystemLogic* vd_system);
	void CopyLinkFile(VdSystemLogic* vd_system);
	void CopyDir(VdSystemLogic* vd_system);
	void CopyDirToDir(VdSystemLogic* vd_system);
private:
	std::string					m_scr_file_string = "";
	std::string					m_dst_file_string = "";
	std::vector<std::string>	m_src_path;
	std::vector<std::string>	m_dst_path;
	VdAbstractFile*				m_scr_file = nullptr;
	VdAbstractFile*				m_dst_file = nullptr;


	VdDirectory*				m_dst_dir = nullptr;
	std::string					m_dst_file_name = "";
};

REGISTER_COMMAND_FACTORY(VdCopyCommand, "copy");
