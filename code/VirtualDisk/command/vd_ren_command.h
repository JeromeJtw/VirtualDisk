/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_ren_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdRenCommand : public VdCommand
{

public:
	VdRenCommand();
	virtual ~VdRenCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

private:
	void ReName(VdSystemLogic* vd_system);

private:
	std::string					m_src_file_name;
	std::vector<std::string>	m_src_path;
	std::string					m_dst_file_name;
};

REGISTER_COMMAND_FACTORY(VdRenCommand, "ren");
