#pragma once
#include "kt.h"
#include "Strings.h"
#include "Array.h"

namespace kt
{

enum FileError
{
	OK,
	Fail,
	FileNotOpen,
	FileDoesntExist
};

bool FileExists(char const* _name);

struct FileStat
{
	char const* m_name;
	uint64_t m_createdTime;
	uint64_t m_lastAccessTime;
	uint64_t m_lastWriteTime;
	uint64_t m_size;
};

// Return false to exit.
using WalkFolderFn = bool(*)(void*, FileStat const&);

bool WalkFolder(char const* _wildcardPath, WalkFolderFn _fn, void* _ctx);


}
