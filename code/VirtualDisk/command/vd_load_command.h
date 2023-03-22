/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_load_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdSystemLogic;
class VdLoadCommand : public VdCommand
{

public:
	VdLoadCommand();
	virtual ~VdLoadCommand();

	void ClearParameter();
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void LoadFromFile(VdSystemLogic* vd_system);
	void CreateNormalFile(const std::string file_info, VdDirectory* parent);
	void CreateDir(const std::string file_info, VdDirectory* parent);
	void CreateLinkFile(const std::string file_info, VdDirectory* parent);
	void PostHandleLinkFile(VdSystemLogic* vd_system,VdDirectory* dir);
private:
	std::string		m_scr_file = "";
	std::string		m_scr_path = "";
	VdDirectory*	m_upper_level_dir = nullptr;
};

REGISTER_COMMAND_FACTORY(VdLoadCommand, "load");
