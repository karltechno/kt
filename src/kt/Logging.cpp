#include <stdio.h>
#include <time.h>

#include "Strings.h"
#include "Logging.h"
#include "Memory.h"
#include "Macros.h"
#include "Concurrency.h"

#if KT_COMPILER_MSVC
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h> // todo: remove
#endif

namespace kt
{

static bool s_willLog[(uint32_t)LogLevel::Num_Levels] = { true, true, true };

static char const* s_logPrefix[(uint32_t)LogLevel::Num_Levels] = { "[Info]: ", "[Warning]: ", "[Error]: " };

struct UserLog
{
	LogCallback m_cb;
	void* m_user;
};

static UserLog s_userLogs[32];
static uint32_t s_numUserLogs = 0;

static Mutex s_userLogMutex;

static bool s_defaultLogSinks[(uint32_t)DefaultLogSink::NumDefaultSinks] = { true, true };

static void LogImpl(LogLevel const _level, char const* _fmt, va_list _list)
{
	if (!s_willLog[(uint32_t)_level])
	{
		return;
	}

	time_t time;
	::time(&time);
	tm* timeInfo = ::localtime(&time);

	char timeBuff[100];

	uint32_t const timeSz = (uint32_t)::strftime(timeBuff, sizeof(timeBuff), "[%d/%m/%y %H:%M:%S]", timeInfo);

	char const* prefix = s_logPrefix[(uint32_t)_level];
	uint32_t const prefixLen = StrLen(prefix);

	uint32_t const allocSz = kt::StringFormatVa(nullptr, 0, _fmt, _list) + 2 + prefixLen + timeSz;

	char* buff = (char*)KT_ALLOCA(allocSz);
	kt::StringFormatVa(buff + prefixLen + timeSz, allocSz - prefixLen - timeSz, _fmt, _list);

	memcpy(buff, timeBuff, timeSz);
	memcpy(buff + timeSz, prefix, prefixLen);
	buff[allocSz - 1] = '\0';
	buff[allocSz - 2] = '\n';

	if (s_numUserLogs)
	{
		s_userLogMutex.Lock();
		KT_SCOPE_EXIT(s_userLogMutex.Unlock());

		for (uint32_t i = 0; i < s_numUserLogs; ++i)
		{
			s_userLogs[i].m_cb(s_userLogs[i].m_user, _level, buff);
		}
	}

	if (s_defaultLogSinks[(uint32_t)DefaultLogSink::DebugOutput])
	{
#if KT_COMPILER_MSVC
		::OutputDebugStringA(buff);
#endif
	}

	if (s_defaultLogSinks[(uint32_t)DefaultLogSink::Console])
	{
		// Remove new line for console. 
		buff[allocSz - 2] = '\0';
		::puts(buff);
	}
}

void Log(LogLevel const _level, char const* _fmt, ...)
{
	va_list l;
	va_start(l, _fmt);
	LogImpl(_level, _fmt, l);
	va_end(l);
}

void LogError(char const* _fmt, ...)
{
	va_list l;
	va_start(l, _fmt);
	LogImpl(LogLevel::Error, _fmt, l);
	va_end(l);
}

void LogWarning(char const* _fmt, ...)
{
	va_list l;
	va_start(l, _fmt);
	LogImpl(LogLevel::Warning, _fmt, l);
	va_end(l);
}

void LogInfo(char const* _fmt, ...)
{
	va_list l;
	va_start(l, _fmt);
	LogImpl(LogLevel::Info, _fmt, l);
	va_end(l);
}

void EnableLogLevel(LogLevel const _level)
{
	s_willLog[(uint32_t)_level] = true;
}
void DisableLogLevel(LogLevel const _level)
{
	s_willLog[(uint32_t)_level] = false;
}

void EnableSink(DefaultLogSink const _sink)
{
	s_defaultLogSinks[(uint32_t)_sink] = true;
}

void DisableSink(DefaultLogSink const _sink)
{
	s_defaultLogSinks[(uint32_t)_sink] = false;
}

void AddSink(LogCallback _cb, void* _ptr)
{
	if (s_numUserLogs == KT_ARRAY_COUNT(s_userLogs))
	{
		KT_ASSERT(false); // ran out of user log space.
	}

	s_userLogs[s_numUserLogs++] = { _cb, _ptr };
}

void RemoveSink(LogCallback _cb, void* _ptr)
{
	for (uint32_t i = 0; i < s_numUserLogs; ++i)
	{
		if (s_userLogs[i].m_cb == _cb && s_userLogs[i].m_user == _ptr)
		{
			s_userLogs[i] = s_userLogs[s_numUserLogs--];
		}
	}
}

}