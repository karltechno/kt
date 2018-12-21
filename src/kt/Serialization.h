#pragma once
#include <kt/kt.h>

namespace kt
{

struct ISerializationReader
{
	virtual ~ISerializationReader() {}

	virtual bool ReadBytes(void* o_buff, uint64_t const _bytesToRead, uint64_t* o_bytesRead = nullptr) = 0;

	template <typename T>
	bool Read(T* o_t)
	{
		return ReadBytes(o_t, sizeof(T));
	}
};

struct ISerializationWriter
{
	virtual ~ISerializationWriter() {}

	virtual bool WriteBytes(void const* o_buff, uint64_t const _bytesToWrite, uint64_t* o_bytesWritten = nullptr) = 0;

	template <typename T>
	bool Write(T const* o_t)
	{
		return WriteBytes(o_t, sizeof(T));
	}
};

}