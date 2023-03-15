/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_mklink_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdMklinkCommand : public VdCommand
{

public:
	VdMklinkCommand();
	virtual ~VdMklinkCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	virtual void Execute(VdSystemLogic* vd_system) override;
	virtual void PrintHelp() override;
	void MakeLink(VdSystemLogic* vd_system);

private:
	std::string			m_src_file_string = "";
	std::string			m_link_string = "";
	VdAbstractFile*		m_src_file = nullptr;
};

REGISTER_COMMAND_FACTORY(VdMklinkCommand, "mklink");
