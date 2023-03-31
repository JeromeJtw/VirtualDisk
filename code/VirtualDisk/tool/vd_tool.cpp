#include "vd_tool.h"
#include <direct.h>
#include <io.h>
#include <sstream>
#include <random>


static std::vector<char> invaild_file_name_char = { '*', '?', ':', '<', '>', '|' };
static std::vector<char> invaild_dir_name_char = { '*', '?', ':', '<', '>', '|', '.' };
std::unordered_map<int, std::string> FILE_TYPE_STRING = { {(int)DIR,"<DIR>"},{(int)NORMALFILE,""},{(int)LINKFILE,"<SYMLINK>"} };


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

		std::string::size_type start_pos = 0;
		auto index = src.find(delimiter);
		while (index != std::string::npos) {
			auto str = src.substr(start_pos, index - start_pos);
			if (str != "") {
				result.push_back(str);
			}
			start_pos = index + delimiter.length();
			index = src.find(delimiter, start_pos);
		}
		// 取最后一个子串
		auto str = src.substr(start_pos);
		if (str != "") {
			result.push_back(str);
		}
		return result;
	}

	std::vector<std::string> SplitStringBySpace(const std::string& str)
	{
		std::vector<std::string> v;
		int len = (int)str.length();
		int start = 0;
		bool is_in_quote = false;
		for (int i = 0; i <= len; ++i)
		{
			if (str[i] == '"')
			{
				is_in_quote = !is_in_quote;
			}
			else if (str[i] == ' ' || str[i] == '\0')
			{
				if (!is_in_quote)
				{
					int word_len = i - start;
					if (word_len > 0)
					{
						std::string substr;
						substr = str.substr(start, word_len);
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
		for (int i = 0; i < string_list.size(); ++i)
		{
			std::string& str = string_list[i];
			for (auto iter = str.begin(); iter != str.end();)
			{
				if ((*iter) == '"')
				{
					iter = str.erase(iter);
					continue;
				}
				iter++;
			}
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

	int IsVaildFileName(const std::string& name)
	{
		if (name.size() > MAX_NAME_LENGTH)
		{
			return TOOLONG;
		}
		for (auto iter : invaild_file_name_char)
		{
			if (name.find(iter) != std::string::npos)
			{
				return HASINVAILDCHAR;
			}
		}
		return VAILDNAME;
	}

	int IsVaildDirName(const std::string& name)
	{
		if (name.size() > MAX_NAME_LENGTH)
		{
			return TOOLONG;
		}
		for (auto iter : invaild_dir_name_char)
		{
			if (name.find(iter) != std::string::npos)
			{
				return HASINVAILDCHAR;
			}
		}
		return VAILDNAME;
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

	void GetFilesByPath(const std::string& path, std::vector<std::string>& files)
	{
		intptr_t  file = 0;
		struct _finddata_t file_info;
		std::string p;
		if ((file = _findfirst(p.assign(path).append("\\*").c_str(), &file_info)) != -1)
		{
			do
			{
				if ((file_info.attrib &  _A_SUBDIR))
				{
					;
				}
				else
				{
					files.push_back(file_info.name);
				}
			} while (_findnext(file, &file_info) == 0);
			_findclose(file);
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
		for (unsigned int i = 0; i < len; ++i) {
			const auto rc = RandomChar();
			std::stringstream hex_stream;
			hex_stream << std::hex << rc;
			auto hex = hex_stream.str();
			ss << (hex.length() < 2 ? '0' + hex : hex);
		}
		return ss.str();
	}

	bool CreateDirectory(const std::string path)
	{
		std::vector<std::string> file_path_dir = SplitString(path, "/");
		std::string path_string = "";
		for (auto file_path_name : file_path_dir)
		{
			path_string += file_path_name + "/";
			if (_access(path_string.c_str(), 0) == -1)
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
