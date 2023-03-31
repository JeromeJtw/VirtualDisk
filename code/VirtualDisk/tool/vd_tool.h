#pragma once
#include "../vd_enum.h"
#include <string>
#include <unordered_map>
#include <vector>
const int MAX_NAME_LENGTH = 256;
const int MAX_PATH_LENGTH = 1024;
const std::string ROOTDISKNAME = "C:";
const std::string CDISKNAME = "C";
const std::string CURRENTDIRNAME = ".";
const std::string PARENTDIRNAME = "..";
const std::string SERIALIZATIONFILESUFFIX = ".vir";
const std::string SERIALIZATIONPREFIX = "*";

namespace VdTool {
	std::vector<std::string> SplitString(const std::string& src, const std::string& delimiter);
	std::vector<std::string> SplitStringBySpace(const std::string& str);
	void ClearQuote(std::vector<std::string>& string_list);
	void BuildRegexByWildcards(std::string& string);
	int IsVaildFileName(const std::string& name);
	int IsVaildDirName(const std::string& name);
	int GetFileTypeByPath(const char* path);
	void GetFilesByPath(const std::string& path, std::vector<std::string>& files);
	unsigned int RandomChar();
	std::string GenerateHex(const unsigned int len);
	bool CreateDirectory(const std::string path);

	template<class T>
	void SafeDeleteSetNull(T*& p);

	template<class T>
	void SafeDeleteSetNull(T*& p)
	{
		if (p)
		{
			delete p;
		}
		p = nullptr;
	}

	template<class T>
	void SafeSetNull(T*& p);

	template<class T>
	void SafeSetNull(T*& p)
	{
		if (p)
		{
			p = nullptr;
		}
	}

}