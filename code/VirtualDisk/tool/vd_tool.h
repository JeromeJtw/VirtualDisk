#pragma once
#include "../vd_enum.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace VdTool {
	std::vector<std::string> SplitString(const std::string& src, const std::string& delimiter);
	std::vector<std::string> SplitStringBySpace(const std::string str);
	void ClearQuote(std::vector<std::string>& string_list);
	void BuildRegexByWildcards(std::string& string);

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

	bool CreateDirectory(const std::string path);
}