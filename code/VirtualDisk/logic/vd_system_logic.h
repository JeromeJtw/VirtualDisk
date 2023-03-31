/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_system_logic.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include <string>
#include <vector>

class VdAbstractFile;
class VdLinkFile;
class VdDirectory;
class VdSystemLogic
{

public:
	~VdSystemLogic();

	static VdSystemLogic* GetVdSystem();

	void InitVdSystem();
	void DestroyVdSystem();
	void PrintInitInfo(bool is_print_current_path = true);
	void HandleCommand(std::string& input_cmd);
	void PrintCurrentPath();
	VdAbstractFile* GetCurrentFile() const;
	void SetCurrentFile(VdAbstractFile* current_file);
	VdAbstractFile* GetActiveFile() const;
	void SetActiveFile(VdAbstractFile* active_file);
	std::vector<std::string> GetCommandPara() const { return m_command_para; }
	bool ChangeActiveDir(const std::string& dir_name);
	VdAbstractFile* GetFileByPath(const std::vector<std::string>& path);
	bool GetFileByPath(std::vector<VdAbstractFile*>& file_list, const std::vector<std::string>& path);
	VdDirectory* GetDiskRoot() const { return m_root; }
	void PostHandleLinkFile(VdLinkFile* link_file);
private:
	explicit VdSystemLogic();
	VdSystemLogic(const VdSystemLogic&) = delete;
	VdSystemLogic(VdSystemLogic&&) = delete;

	void CreateRootDir();
	void SplitCommandString(const std::string& src, const std::string& delimiter);
	std::string GetCommandName();
	void RecursionDestroyFile(VdAbstractFile* file);

private:
	VdDirectory*				m_root = nullptr;
	VdAbstractFile*				m_current_file_node = nullptr;
	VdAbstractFile*				m_active_file_node = nullptr;
	std::vector<std::string>	m_command_para;
};
