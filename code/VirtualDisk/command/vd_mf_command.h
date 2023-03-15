/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_mf_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdMfCommand : public VdCommand
{

public:
	VdMfCommand();
	virtual ~VdMfCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void CreateFile(VdSystemLogic* vd_system);
private:
	std::string m_file_name = "";
	int m_file_size = 0;
};

REGISTER_COMMAND_FACTORY(VdMfCommand, "mf");
