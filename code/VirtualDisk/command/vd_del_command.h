/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_del_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdDelCommand : public VdCommand
{

public:
	VdDelCommand();
	virtual ~VdDelCommand();

	bool ParseParameter(VdSystemLogic* vd_system) override;
	void ClearParameter() override;
	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;
	void DeleteFile(VdSystemLogic* vd_system);
	void RecursionDelete(VdSystemLogic* vd_system);
	void RecursionDeleteFile(VdSystemLogic* vd_system, VdDirectory* work_dir, const std::string vague_name);
	void PrintResult(const std::string file_name, const int result, const bool is_dir);
private:
	std::vector<std::string>	m_dst_file_list;
	bool						m_recursion_delete = false;
	bool						m_delete_all_file = false;
};

REGISTER_COMMAND_FACTORY(VdDelCommand, "del");
