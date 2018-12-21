#pragma once
#include "kt.h"

#define KT_LOG_ERROR(fmt, ...) kt::LogError(fmt, __VA_ARGS__)
#define KT_LOG_WARNING(fmt, ...) kt::LogWarning(fmt, __VA_ARGS__)
#define KT_LOG_INFO(fmt, ...) kt::LogInfo(fmt, __VA_ARGS__)

namespace kt
{

enum class LogLevel : uint32_t
{
	Info,
	Warning,
	Error,
	Num_Levels
};

enum class DefaultLogSink : uint32_t
{
	Console,
	DebugOutput,
	NumDefaultSinks
};

typedef void (*LogCallback)(void* _user, LogLevel _level, char const* _str);

void Log(LogLevel const _level, char const* _fmt, ...);

void LogError(char const* _fmt, ...);
void LogWarning(char const* _fmt, ...);
void LogInfo(char const* _fmt, ...);

void EnableLogLevel(LogLevel const _level);
void DisableLogLevel(LogLevel const _level);

void EnableSink(DefaultLogSink const _sink);
void DisableSink(DefaultLogSink const _sink);

void AddSink(LogCallback cb, void* _ptr);
void RemoveSink(LogCallback cb, void* _ptr);

}