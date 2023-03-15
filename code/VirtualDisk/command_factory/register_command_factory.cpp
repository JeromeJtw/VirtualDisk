#include "register_command_factory.h"

RegisterCommandFactory::RegisterCommandFactory()
{

}

RegisterCommandFactory& RegisterCommandFactory::GetInstance()
{
	static RegisterCommandFactory instance;
	return instance;
}

RegisterCommandFactory::~RegisterCommandFactory()
{

}

VdCommand* RegisterCommandFactory::GetCommandByName(const std::string command_name)
{
	if (m_command_map.find(command_name) != m_command_map.end())
	{
		return m_command_map[command_name]();
	}
	return nullptr;
}
