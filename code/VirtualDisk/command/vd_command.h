/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "../command_factory/register_command_factory.h"
#include <string>
#include <vector>

class VdSystemLogic;
struct CommandParameter;
class VdCommand
{

public:
	VdCommand();
	virtual ~VdCommand();

	virtual bool ParseParameter(VdSystemLogic* vd_system);
	virtual void ClearParameter();
	virtual void PreExecute();
	virtual void Execute(VdSystemLogic* vd_system);
	virtual void PostExecute(VdSystemLogic* vd_system);
	virtual void PrintHelp();
};

