#pragma once
#include "kt.h"
#include "Array.h"
#include "HashMap.h"

namespace kt
{

struct IReader
{
	virtual ~IReader() {}

	virtual bool ReadBytes(void* o_buff, uint64_t const i_bytesToRead, uint64_t* o_bytesRead = nullptr) = 0;

	template <typename T>
	bool Read(T& o_t)
	{
		return ReadBytes(&o_t, sizeof(T));
	}
};

struct IWriter
{
	virtual ~IWriter() {}

	virtual bool WriteBytes(void const* i_buff, uint64_t const i_bytesToWrite, uint64_t* o_bytesWritten = nullptr) = 0;

	template <typename T>
	bool Write(T const& o_t)
	{
		return WriteBytes(&o_t, sizeof(T));
	}
};

struct FileWriter : IWriter
{
	FileWriter(FILE* _file)
		: m_file(_file)
	{}

	bool WriteBytes(void const* i_buff, uint64_t const _bytesToWrite, uint64_t* o_bytesWritten = nullptr) override;

	FILE* m_file = nullptr;
};

struct FileReader : IReader
{
	FileReader(FILE* _file)
		: m_file(_file)
	{}

	bool ReadBytes(void* o_buff, uint64_t const i_bytesToRead, uint64_t* o_bytesRead = nullptr) override;

	FILE* m_file = nullptr;
};

struct StaticMemoryBlockWriter : IWriter
{
	StaticMemoryBlockWriter(void* _buff, size_t _sz)
		: m_buff(_buff)
		, m_size(_sz)
		, m_pos(0)
	{}

	bool WriteBytes(void const* i_buff, uint64_t const _bytesToWrite, uint64_t* o_bytesWritten = nullptr) override;

	void* m_buff = nullptr;
	uint64_t m_size = 0;
	uint64_t m_pos = 0;
};

struct StaticMemoryBlockReader : IReader
{
	StaticMemoryBlockReader(void* _buff, size_t _sz)
		: m_buff(_buff)
		, m_size(_sz)
		, m_pos(0)
	{}

	bool ReadBytes(void* o_buff, uint64_t const i_bytesToRead, uint64_t* o_bytesRead = nullptr) override;

	void* m_buff = nullptr;
	uint64_t m_size = 0;
	uint64_t m_pos = 0;
};


struct ISerializer
{
	enum class Mode : uint32_t
	{
		Write,
		Read
	};

	ISerializer(IReader* _reader, uint32_t _version)
		: m_mode(Mode::Read)
		, m_version(_version)
		, m_reader(_reader)
	{}

	ISerializer(IWriter* _writer, uint32_t _version)
		: m_mode(Mode::Write)
		, m_version(_version)
		, m_writer(_writer)
	{}

	Mode SerializeMode() const { return m_mode; }

	void SerializeBytes(void* _buff, uint64_t _sz)
	{
		bool const ok = m_mode == Mode::Write ? m_writer->WriteBytes(_buff, _sz) : m_reader->ReadBytes(_buff, _sz);
		KT_ASSERT(ok);
		m_ok &= ok;
	}

	uint32_t Version() const { return m_version; }

	bool OK() const { return m_ok; }

private:
	Mode m_mode;
	uint32_t m_version;

	union 
	{
		IReader* m_reader;
		IWriter* m_writer;
	};

	bool m_ok = true;
};

template <typename T>
void Serialize(ISerializer* _s, T& _t)
{
	_s->SerializeBytes(&_t, sizeof(T));
}

template <typename T>
void Serialize(ISerializer* _s, kt::Array<T>& _t);

template <typename K, typename V, typename KeyOps>
void Serialize(ISerializer* _s, kt::HashMap<K, V, KeyOps>& _t);

template <uint32_t SizeT>
void Serialize(ISerializer* _s, kt::StaticString<SizeT>& _t);

}

#include "inl/Serialization.inl"