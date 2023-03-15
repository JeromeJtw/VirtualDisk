#include "vd_tool.h"
#include <direct.h>
#include <io.h>

namespace VdTool {
	std::vector<std::string> SplitString(const std::string& src, const std::string& delimiter)
	{
		std::vector<std::string> result;
		// 入参检查
		// 1.原字符串为空或等于分隔符，返回空 vector
		if (src == "" || src == delimiter) {
			return result;
		}
		// 2.分隔符为空返回单个元素为原字符串的 vector
		if (delimiter == "") {
			result.push_back(src);
			return result;
		}

		std::string::size_type startPos = 0;
		auto index = src.find(delimiter);
		while (index != std::string::npos) {
			auto str = src.substr(startPos, index - startPos);
			if (str != "") {
				result.push_back(str);
			}
			startPos = index + delimiter.length();
			index = src.find(delimiter, startPos);
		}
		// 取最后一个子串
		auto str = src.substr(startPos);
		if (str != "") {
			result.push_back(str);
		}
		return result;
	}

	std::vector<std::string> SplitStringBySpace(const std::string str)
	{
		std::vector<std::string> v;
		int len = (int)str.length();
		int start = 0;
		bool inQuote = false;
		for (int i = 0; i <= len; i++)
		{
			//if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
			if (str[i] == '"')
			{
				inQuote = !inQuote;
			}
			//else if (isspace(str[i]) || str[i] == '\0')
			else if (str[i] == ' ' || str[i] == '\0')
			{
				if (!inQuote)
				{
					int wordLen = i - start;
					if (wordLen > 0)
					{
						std::string substr;
						substr = str.substr(start, wordLen);
						v.push_back(substr);
					}
					start = i + 1;
				}
			}
		}
		return v;
	}

	void ClearQuote(std::vector<std::string>& string_list)
	{
		for (int i = 0; i < string_list.size(); i++)
		{
			std::string str = string_list[i];
			for (auto iter = str.begin(); iter != str.end();)
			{
				if ((*iter) == '"')
				{
					iter = str.erase(iter);
					continue;
				}
				iter++;
			}
			string_list[i] = str;
		}
	}

	void BuildRegexByWildcards(std::string& string)
	{
		auto iter1 = std::find(string.begin(), string.end(), '?');
		if (iter1 != string.end())
		{
			if (iter1 == string.begin())
			{
				string.erase(string.begin());
				string.insert(string.begin(), '.');
				return;
			}
			else
			{
				string.insert(iter1, '.');
				return;
			}
		}

		auto iter2 = std::find(string.begin(), string.end(), '*');
		if (iter2 != string.end())
		{
			if (iter2 == string.end() - 1)
			{
				string.insert(string.end() - 1, '.');
				return;
			}
			else
			{
				iter2 = string.erase(iter2);
				string.insert(iter2, '+');
				string.insert(iter2, '.');
				return;
			}
		}
	}

	bool CreateDirectory(const std::string path)
	{
		std::vector<std::string> file_path_dir = SplitString(path, "/");
		std::string path_string = "";
		for (int i = 0; i < file_path_dir.size(); i++)
		{
			path_string += file_path_dir[i] + "/";
			if(_access(path_string.c_str(),0) == -1)
			{
				if (mkdir(path_string.c_str()) == -1)
				{
					return false;
				}
			}
		}
		return true;
	}
}

std::unordered_map<int, std::string> FILE_TYPE_STRING = { {(int)DIR,"<DIR>"},{(int)NORMALFILE,""},{(int)LINKFILE,"<SYMLINK>"} };
