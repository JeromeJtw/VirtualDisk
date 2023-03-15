/*********************************************************************
 * Created:		2023/03/03
 * File name:	register_command_factory.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/
#pragma once
#include <functional>
#include <unordered_map>

class VdCommand;
typedef std::function<VdCommand*()> CreateCommandFunc;
class RegisterCommandFactory
{

public:
	~RegisterCommandFactory();
	template<class T>
	class RegisterCommand
	{
	public:
		RegisterCommand(std::string command_class_name)
		{
			RegisterCommandFactory::GetInstance().m_command_map.emplace(command_class_name, [] {return new T(); });
		}
	};
	static RegisterCommandFactory& GetInstance();
	VdCommand* GetCommandByName(const std::string command_name);
private:
	RegisterCommandFactory();
	RegisterCommandFactory(const RegisterCommandFactory& register_command_factory) = delete;
	RegisterCommandFactory(RegisterCommandFactory&& register_command_factory) = delete;
	std::unordered_map<std::string, CreateCommandFunc> m_command_map;
};

#define COMMAND_NAME(T) RegisterCommand_##T
#define REGISTER_COMMAND_FACTORY(T,key) static RegisterCommandFactory::RegisterCommand<T> COMMAND_NAME(T)(key)