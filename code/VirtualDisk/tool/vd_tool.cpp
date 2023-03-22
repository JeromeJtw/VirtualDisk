#include "vd_tool.h"
#include <direct.h>
#include <io.h>
#include <sstream>
#include <random>

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
			if (str[i] == '"')
			{
				inQuote = !inQuote;
			}
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

	bool IsVaildFileName(const std::string& name)
	{
		if (name.find('*') != std::string::npos || name.find('?') != std::string::npos ||
			name.find(':') != std::string::npos || name.find('<') != std::string::npos ||
			name.find('>') != std::string::npos || name.find('|') != std::string::npos )
		{
			return false;
		}
		return true;
	}

	bool IsVaildDirName(const std::string& name)
	{
		if (name.find('*') != std::string::npos || name.find('?') != std::string::npos ||
			name.find(':') != std::string::npos || name.find('<') != std::string::npos ||
			name.find('>') != std::string::npos || name.find('|') != std::string::npos ||
			name.find('.') != std::string::npos)
		{
			return false;
		}
		return true;
	}

	int GetFileTypeByPath(const char* path)
	{
		struct stat s;
		if (stat(path, &s) == 0)
		{
			if (s.st_mode & S_IFDIR)
			{
				return ISDIR;
			}
			else if (s.st_mode & S_IFREG)
			{
				return ISFILE;
			}
			else
			{
				return UNKOWN;
			}
		}
		else
		{
			return ERR;
		}
	}

	void GetFilesByPath(std::string path, std::vector<std::string>& files)
	{
		intptr_t  hFile = 0;
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					;
				}
				else
				{
					files.push_back(fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	unsigned int RandomChar()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);
		return dis(gen);
	}

	std::string GenerateHex(const unsigned int len)
	{
		std::stringstream ss;
		for (unsigned int i = 0; i < len; i++) {
			const auto rc = RandomChar();
			std::stringstream hexstream;
			hexstream << std::hex << rc;
			auto hex = hexstream.str();
			ss << (hex.length() < 2 ? '0' + hex : hex);
		}
		return ss.str();
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

