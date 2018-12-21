#include "catch.hpp"

#include <kt/kt.h>
#include <kt/FilePath.h>

static char* ReplacePlatformSeperators(char* _buff)
{
	char* ret = _buff;
#if KT_PLATFORM_WINDOWS
	char const platsep = '\\';
	char const replace = '/';
#else
	char const platsep = '/';
	char const replace = '\\';
#endif
	while (*_buff)
	{
		if (*_buff == replace)
		{
			*_buff = platsep;
		}
		++_buff;
	}

	return ret;
}

TEST_CASE("File Path 1", "[File Path]")
{
	char path[] = "cats/Hi.txt";
	ReplacePlatformSeperators(path);


	kt::FilePath filePath(path);

	CHECK(filePath.GetStringView() == path);
}

TEST_CASE("Double slash at start", "[File Path]")
{
	char path[] = "//cats/Hi.txt";
	ReplacePlatformSeperators(path);
	char realPath[] = "/cats/Hi.txt";
	ReplacePlatformSeperators(realPath);

	kt::FilePath filePath(path);

	CHECK(filePath.GetStringView() == realPath);
}

TEST_CASE(".. in middle", "[File Path]")
{
	char path[] = "/cats/badFolder/../Hi.txt";
	ReplacePlatformSeperators(path);
	char realPath[] = "/cats/Hi.txt";
	ReplacePlatformSeperators(realPath);

	kt::FilePath filePath(path);

	CHECK(filePath.GetStringView() == realPath);
}

TEST_CASE("multiple .. in middle", "[File Path]")
{
	char path[] = "/cats/badFolder/../badfolder2/../badfolder3/../Hi.txt";
	ReplacePlatformSeperators(path);
	char realPath[] = "/cats/Hi.txt";
	ReplacePlatformSeperators(realPath);

	kt::FilePath filePath(path);

	CHECK(filePath.GetStringView() == realPath);
	kt::StringView const actualPath = filePath.GetPath();
	int x = 5;
	KT_UNUSED(x);
}

TEST_CASE("Path info extraction 1", "[File Path]")
{
	{
		char path[] = "//cats/Hi.txt";
		kt::FilePath filePath(path);

		CHECK(filePath.GetFileName() == "Hi.txt");
		CHECK(filePath.GetExtension() == ".txt");
	}

	{
		char path[] = "//cats/Hi.txt";
		kt::FilePath filePath(path);

		char pathNoFile[] = "/cats/";
		ReplacePlatformSeperators(pathNoFile);

		CHECK(filePath.GetFileName() == "Hi.txt");
		CHECK(filePath.GetExtension() == ".txt");
		CHECK(filePath.GetPath() == pathNoFile);
	}
}