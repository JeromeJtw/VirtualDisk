/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_md_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdMdCommand : public VdCommand
{

public:
	VdMdCommand();
	virtual ~VdMdCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

private:
	void MakeDir(VdSystemLogic* vd_system);

private:
	std::vector<std::string> m_dir_name_list;
};

REGISTER_COMMAND_FACTORY(VdMdCommand, "md");
