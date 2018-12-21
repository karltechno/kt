#include "File.h"
#include "Platform.h"

#if KT_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <shlwapi.h>
	#include <handleapi.h>
#elif KT_PLATFORM_POSIX
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
#endif

namespace kt
{

#if KT_PLATFORM_WINDOWS
static void Win32FindDataToFileInfo(FileStat& o_fileInfo, WIN32_FIND_DATAA const& _findData)
{
	o_fileInfo.m_name = _findData.cFileName;
	o_fileInfo.m_createdTime = *(uint64_t*)&_findData.ftCreationTime;
	o_fileInfo.m_lastAccessTime = *(uint64_t*)&_findData.ftLastAccessTime;
	o_fileInfo.m_lastWriteTime = *(uint64_t*)&_findData.ftLastWriteTime;
	o_fileInfo.m_size = ((uint64_t)_findData.nFileSizeLow) | ((uint64_t)_findData.nFileSizeHigh << 32u);
}

bool WalkFolderImplWindows(char const* _path, WalkFolderFn _fn, void* _ctx)
{
	WIN32_FIND_DATAA findData;
	HANDLE handle = ::FindFirstFileA(_path, &findData);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FileStat fileInfo;
	Win32FindDataToFileInfo(fileInfo, findData);
	BOOL ret;
	do
	{
		if (!_fn(_ctx, fileInfo))
		{
			return true;
		}

		ret = ::FindNextFileA(handle, &findData);
	} while (ret != 0);
	return true;
}
#endif

bool FileExists(char const* _name)
{
#if KT_PLATFORM_WINDOWS
	return ::PathFileExistsA(_name);
#elif KT_PLATFORM_POSIX
	struct stat buf;
	int status = stat(_name, &buf);
	return status == 1;
#endif
}


bool WalkFolder(char const* _path, WalkFolderFn _fn, void* _ctx)
{
#if KT_PLATFORM_WINDOWS
	return WalkFolderImplWindows(_path, _fn, _ctx);
#endif
}



}
