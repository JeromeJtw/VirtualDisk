/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_cd_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"
#include "../command_factory/register_command_factory.h"

class VdCdCommand : public VdCommand
{

public:
	VdCdCommand();
	virtual ~VdCdCommand();

	bool ParseParameter(VdSystemLogic* vd_system) override;
	void ClearParameter() override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

private:
	bool ChangeCurrentDir(VdSystemLogic* vd_system, const std::string& dir_name);

private:
	std::string m_dst_path;
};

REGISTER_COMMAND_FACTORY(VdCdCommand, "cd");