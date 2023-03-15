/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_dir_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdDirCommand : public VdCommand
{

public:
	VdDirCommand();
	virtual ~VdDirCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void PrintCurrentDir(VdSystemLogic* vd_system);
	void NormalPrint(VdSystemLogic* vd_system);
	void RecursionPrint(VdSystemLogic* vd_system);
	void PrintDir(VdSystemLogic* vd_system, std::string dir);
private:
	std::string		m_dst_dir = "";
	bool			m_recursion_print = false;
	bool			m_print_dir = false;
};

REGISTER_COMMAND_FACTORY(VdDirCommand, "dir");
