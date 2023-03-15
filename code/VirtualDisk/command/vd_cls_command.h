/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_cls_command.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_command.h"

class VdClsCommand : public VdCommand
{

public:
	VdClsCommand();
	virtual ~VdClsCommand();

	void Execute(VdSystemLogic* vd_system) override;
	void PrintHelp() override;

};

REGISTER_COMMAND_FACTORY(VdClsCommand, "cls");