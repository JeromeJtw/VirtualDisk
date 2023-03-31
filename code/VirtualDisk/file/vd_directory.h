/*********************************************************************
 * Created:		2023/03/03
 * File name:	vd_directory.h
 * Author:		jiantingwu
 * Revision:	1.0.0
 *
 * Copyright (C) 2010 - 2023 Horizon3D, All Rights Reserved.
 *
 * Purpose: 
 *********************************************************************/

#pragma once
#include "vd_abstract_file.h"
#include <vector>

class VdAbstractFile;
class VdDirectory : public VdAbstractFile
{

public:
	VdDirectory(std::string name, int file_type);
	virtual ~VdDirectory();

	void PrintPath() override;
	int AddAbstractFile(VdAbstractFile* vd_abstract_file) override;
	int RemoveSubDirByName(const std::string& name);
	void RemoveSubDir(VdAbstractFile* dir);
	//ֻ�����ļ��б����Ƴ����ļ���������
	void EraseSubFileByName(const std::string& name);
	//�������ļ��б����Ƴ����ļ�����Ҳ����
	int DeleteSubFileByName(const std::string& name);
	//ɾ������normalfile/linkfile������ɾ��dir
	void DeleteAllNormalFile();
	void RecursionDeleteSubFileByName(const std::string& name,const bool is_print_info = true);
	//ֻɾ���ļ����ڵ��ļ����ļ����ڵ��ļ��в�ɾ��
	void RecursionDeleteAllFile();
	//�����ļ��У������ļ����ڵ��ļ���
	void RecursionDestoryDir();
	std::vector<VdAbstractFile*> GetSubFileList() const { return m_file_list; };
	std::vector<VdAbstractFile*> GetNormalSubFileList() const;
	std::vector<VdAbstractFile*> GetNormalSubFileByVagueName(const std::string& vague_name) const;
	std::vector<VdDirectory*> GetDir() const;
	VdAbstractFile* GetSubFileByName(const std::string& name);
	void RecursionPrintFileInfo(bool is_only_print_dir, int& file_count, int& dir_count);
	bool IsExistFile(const std::string& name);
	bool IsExistFile(VdAbstractFile* file);
	bool IsExistDir(const std::string& name);

private:
	void AddDefaultDir();

private:
	std::vector<VdAbstractFile*> m_file_list;
};
