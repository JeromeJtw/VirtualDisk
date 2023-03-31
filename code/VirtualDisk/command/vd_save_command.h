/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_save_command.h
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
#include <fstream>

class VdSaveCommand : public VdCommand
{

public:
	VdSaveCommand();
	virtual ~VdSaveCommand();

	void ClearParameter() override;
	bool ParseParameter(VdSystemLogic* vd_system) override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

private:
	void Save(VdSystemLogic* vd_system);
	void SaveDir(VdDirectory* dir, std::string prefix);
	void SaveFile(VdAbstractFile* save_file, std::string prefix);

private:
	std::string			m_dst_file;
	std::string			m_dst_path;
	std::ofstream		m_file;
};

REGISTER_COMMAND_FACTORY(VdSaveCommand, "save");
