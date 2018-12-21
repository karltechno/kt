#pragma once

#define KT_PLATFORM_WINDOWS		(0)
#define KT_PLATFORM_OSX			(0)
#define KT_PLATFORM_IOS			(0)
#define	KT_PLATFORM_LINUX		(0)
#define KT_PLATFORM_POSIX		(0)

#define KT_COMPILER_MSVC		(0)
#define KT_COMPILER_CLANG		(0)
#define KT_COMPILER_GCC			(0)

#define KT_CPU_X86				(0)
#define KT_CPU_ARM				(0)

#define KT_ARCH_64BIT			(0)
#define KT_ARCH_32BIT			(0)

#if defined(__clang__)
	#undef KT_COMPILER_CLANG
	#define KT_COMPILER_CLANG	(1)
#elif defined(__GNUG__)
	#undef KT_COMPILER_GCC
	#define KT_COMPILER_GCC		(1)
#elif defined(_MSC_VER)
	#undef KT_COMPILER_MSVC
	#define KT_COMPILER_MSVC	(1)
#else
	#error Compiler could not be detected.
#endif

#if defined(__arm__) || defined(_ARM) || defined(_M_ARM) || defined(__arch64__)
	#undef KT_CPU_ARM
	#define KT_CPU_ARM			(1)
#elif defined (_M_IX86) || defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64) || defined(__amd64__)
	#undef KT_CPU_X86
	#define KT_CPU_X86			(1)
#else
	#error CPU type could not be detected/unsupported.
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__)
	#undef KT_ARCH_64BIT 
	#define KT_ARCH_64BIT		(1)
#else
	#undef KT_ARCH_32BIT
	#define KT_ARCH_32BIT		(1)
#endif

#if KT_COMPILER_MSVC
	#if !defined(NOMINMAX)
		#define NOMINMAX
	#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
	#undef KT_PLATFORM_WINDOWS
	#define KT_PLATFORM_WINDOWS	(1)
#elif defined(__linux__)
	#undef KT_PLATFORM_LINUX
	#define KT_PLATFORM_LINUX	(1)
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
	#undef KT_PLATFORM_OSX
	#define KT_PLATFORM_OSX		(1)
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
	#undef KT_PLATFORM_IOS
	#define KT_PLATFORM_IOS		(1)
#endif

#if KT_PLATFORM_LINUX || KT_PLATFORM_IOS || KT_PLATFORM_LINUX
	#undef KT_PLATFORM_POSIX
	#define KT_PLATFORM_POSIX	(1)
#endif
