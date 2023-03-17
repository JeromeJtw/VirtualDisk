#pragma once
#include <string>
#include <unordered_map>
enum VdFileType
{
	DIR = 1,
	NORMALFILE,
	LINKFILE
};

enum REMOVERESULT
{
	SUCCESSED = 0,
	NOTEMPTY,
	ISUSING,
	NOTFIND,
	CANNOTRM
};

enum PhysicalFileType
{
	ISDIR = 1,
	ISFILE,
	UNKOWN,
	ERR
};