/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_move_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"
class VdAbstractFile;
class VdMoveCommand : public VdCommand
{

public:
	VdMoveCommand();
	virtual ~VdMoveCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system);
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

private:
	void MoveFile(VdSystemLogic* vd_system);
	void MoveDir(VdSystemLogic* vd_system);
	void MoveNormalFile(VdSystemLogic* vd_system);

private:
	bool							m_is_override = false;
	std::string						m_src_file_string;
	std::string						m_dst_file_string;
	std::vector<std::string>		m_src_path;
	std::vector<std::string>		m_dst_path;
	VdAbstractFile*					m_src_file = nullptr;
	VdAbstractFile*					m_dst_file = nullptr;

};

REGISTER_COMMAND_FACTORY(VdMoveCommand, "move");
