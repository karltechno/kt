#pragma once
#include "kt.h"
#include "Strings.h"

namespace kt
{

class FilePath
{
public:
	static const uint32_t MAX_PATH_LEN = 1024;
	using FilePathString = StaticString<MAX_PATH_LEN>;

	FilePath() = default;
	FilePath(char const* _str);
	FilePath(StringView const& _view);

	static FilePath WorkingDirectory();

	StringView GetFileName() const;
	StringView GetExtension() const;
	StringView GetFileNameNoExt() const;
	StringView GetPath() const;

	void Set(StringView const& _view);

	void Append(StringView const& _view);

	bool IsAbsolute() const;
	bool Empty() const;

	void Clear();

	char const* Data() const; 
	StringView GetStringView() const;

private:
	FilePathString m_str;
};

}