/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_rd_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdRdCommand : public VdCommand
{

public:
	VdRdCommand();
	virtual ~VdRdCommand();

	void Execute(VdSystemLogic* vd_system) override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void ClearParameter() override;

	bool RemoveDir(VdSystemLogic* vd_system);
	void RecursionRemoveDir(VdSystemLogic* vd_system);
private:
	bool m_is_recursion_delete = false;
	std::vector<std::string> m_dst_file_list;
};

REGISTER_COMMAND_FACTORY(VdRdCommand, "rd");
