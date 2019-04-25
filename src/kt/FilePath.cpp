#include "FilePath.h"
#include <stdio.h>

#if KT_PLATFORM_WINDOWS
	#include <direct.h>
#elif KT_PLATFORM_POSIX
	#include <unistd.h>
#endif

namespace kt
{

#if KT_PLATFORM_WINDOWS
static char const PLATFORM_PATH_SEP = '\\';
#else
static char const PLATFORM_PATH_SEP = '/';
#endif

static bool IsPathSep(char const _c)
{
	return (_c == '\\') || (_c == '/');
}


static uint32_t NormalizePath(char* _buff, uint32_t const _buffLen)
{
#define PATH_SEP_OR_END(c, pos, maxLen) (!c || IsPathSep(c) || pos >= maxLen)
	// lib9/cleanname.c as reference

	if (!_buffLen || !_buff)
	{
		return 0;
	}

	uint32_t rooted = IsPathSep(_buff[0]) ? 1 : 0;
	if (rooted)
	{
		_buff[0] = PLATFORM_PATH_SEP;
	}

	uint32_t pos = rooted;
	uint32_t dotDotPos = rooted;
	uint32_t lastWritten = rooted;

	while (pos < _buffLen && _buff[pos])
	{
		switch (_buff[pos])
		{
			case '/':
			case '\\':
			{
				_buff[pos++] = PLATFORM_PATH_SEP;
			} break;

			case '.':
			{
				if (pos + 1 == _buffLen || IsPathSep(_buff[pos + 1]))
				{
					++pos;
					break;
				}
				else if (_buff[pos + 1] == '.' && PATH_SEP_OR_END(_buff[pos + 2], pos, _buffLen))
				{
					pos += 2;

					if (lastWritten > dotDotPos)
					{
						while (--lastWritten > dotDotPos && !IsPathSep(_buff[lastWritten])) { /* */ }
					}
					else if (!rooted)
					{
						if (lastWritten != 0)
						{
							_buff[lastWritten++] = PLATFORM_PATH_SEP;
						}
						_buff[lastWritten++] = '.';
						_buff[lastWritten++] = '.';
						dotDotPos = lastWritten;
					}

					break;
				}
			} KT_FALLTHROUGH;

			default:
			{
				if (lastWritten != rooted)
				{
					_buff[lastWritten++] = PLATFORM_PATH_SEP;
				}

				for (;;)
				{
					char c = _buff[pos];
					_buff[lastWritten] = c;

					if (PATH_SEP_OR_END(c, pos, _buffLen))
					{
						break;
					}
					++lastWritten;
					++pos;
				}

			} break;
		}
	}

	if (lastWritten == 0)
	{
		_buff[lastWritten++] = '.';
	}

	_buff[Min(lastWritten, _buffLen)] = 0;
	return lastWritten;
#undef PATH_SEP_OR_END
}

FilePath::FilePath(char const* _str)
{
	Set(StringView(_str));
}

FilePath::FilePath(StringView const& _view)
{
	Set(_view);
}

FilePath FilePath::WorkingDirectory()
{
	char buff[FilePath::MAX_PATH_LEN];
#if KT_PLATFORM_WINDOWS
	::_getcwd(buff, FilePath::MAX_PATH_LEN);
	return FilePath(buff);
#else
	::getcwd(buff, sizeof(buff));
	return FilePath(buff);
#endif
}

StringView FilePath::GetFileName() const
{
	StringView const pathEnd = StrFindR(m_str, PLATFORM_PATH_SEP);
	if (!pathEnd.Empty())
	{
		return StringView(pathEnd.Begin() + 1, pathEnd.End());
	}

	return GetStringView();
}

StringView FilePath::GetExtension() const
{
	return StrFind(m_str, '.');
}

StringView FilePath::GetFileNameNoExt() const
{
	StringView const fileName = GetFileName();
	StringView const ext = StrFind(fileName, '.');

	if (!ext.Empty())
	{
		return StringView(fileName.Begin(), ext.Begin());
	}

	return fileName;
}

StringView FilePath::GetPath() const
{
	StringView const pathEnd = StrFindR(m_str, PLATFORM_PATH_SEP);

	if (!pathEnd.Empty())
	{
		return StringView(m_str.Data(), pathEnd.Begin() + 1);
	}

	return StringView();
}

void FilePath::Set(StringView const& _str)
{
	m_str = _str;
	uint32_t const wouldHaveWritten = NormalizePath(m_str.Data(), MAX_PATH_LEN);
	m_str.Resize(wouldHaveWritten ? wouldHaveWritten : 0);
}


void FilePath::Append(StringView const& _view)
{
	FilePathString tempStr = m_str;
	tempStr.Append(PLATFORM_PATH_SEP);
	tempStr.Append(_view);
	Set(tempStr);
}

bool FilePath::IsAbsolute() const
{
	char const* ptr = m_str.Data();
	static_assert(MAX_PATH_LEN >= 3, "This is insanity");

	return ptr[0] == PLATFORM_PATH_SEP || (ptr[1] == ':' && ptr[2] == PLATFORM_PATH_SEP);
}

bool FilePath::Empty() const
{
	return m_str.Empty();
}

void FilePath::Clear()
{
	m_str.Clear();
}

char* FilePath::DataMutable()
{
	return m_str.Data();
}

char const* FilePath::Data() const
{
	return m_str.Data();
}

StringView FilePath::GetStringView() const
{
	return StringView(m_str);
}

}