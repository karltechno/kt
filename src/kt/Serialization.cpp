#include "Serialization.h"

namespace kt
{

bool StaticMemoryBlockWriter::WriteBytes(void const* _buff, uint64_t const i_bytesToWrite, uint64_t* o_bytesWritten )
{
	KT_ASSERT(m_pos <= m_size);

	uint64_t const toWrite = kt::Min(m_size - m_pos, i_bytesToWrite);
	if (toWrite) 
	{
		memcpy((void*)(uintptr_t(m_buff) + m_pos), _buff, toWrite); m_pos += toWrite; 
	}

	if (o_bytesWritten) 
	{ 
		*o_bytesWritten = toWrite; 
	}

	return toWrite == i_bytesToWrite;
}

bool FileWriter::WriteBytes(void const* i_buff, uint64_t const i_bytesToWrite, uint64_t* o_bytesWritten /*= nullptr*/)
{
#if KT_PLATFORM_WINDOWS
	size_t const written = _fwrite_nolock(i_buff, 1, i_bytesToWrite, m_file);
#else
	size_t const written = fwrite(i_buff, 1, i_bytesToWrite, m_file);
#endif

	if (o_bytesWritten)
	{
		*o_bytesWritten = written;
	}

	return i_bytesToWrite == written;
}

void FileWriter::SeekTo(int64_t _offset, Origin _origin)
{
	switch (_origin)
	{
#if KT_PLATFORM_WINDOWS
		case ISeeker::Origin::Absolute: _fseeki64_nolock(m_file, _offset, SEEK_SET); break;
		case ISeeker::Origin::Relative: _fseeki64_nolock(m_file, _offset, SEEK_CUR); break;
#else
		case ISeeker::Origin::Absolute: fseek(m_file, int32_t(_offset), SEEK_SET); break;
		case ISeeker::Origin::Relative: fseek(m_file, int32_t(_offset), SEEK_CUR); break;
#endif
	}
}

uint64_t FileWriter::CurrentPos() const
{
#if KT_PLATFORM_WINDOWS
	return _ftelli64_nolock(m_file);
#else
	return uint64_t(ftell(m_file));
#endif
}

FileReader::FileReader(FILE* _file)
	: m_file(_file)
{
	// TODO: 64 bit
	uint32_t const pos = ftell(m_file);
	fseek(m_file, 0, SEEK_END);

#if KT_PLATFORM_WINDOWS
	size_t const end = _ftelli64_nolock(m_file);
#else
	uint64_t const end = ftell(m_file);
#endif

	m_size = end - pos;
	fseek(m_file, pos, SEEK_SET);
}

bool FileReader::ReadBytes(void* o_buff, uint64_t const i_bytesToRead, uint64_t* o_bytesRead /*= nullptr*/)
{
#if KT_PLATFORM_WINDOWS
	size_t const read = _fread_nolock(o_buff, 1, i_bytesToRead, m_file);
#else
	size_t const read = fread(o_buff, 1, i_bytesToRead, m_file);
#endif
	if (o_bytesRead)
	{
		*o_bytesRead = read;
	}

	return i_bytesToRead == read;
}

bool StaticMemoryBlockReader::ReadBytes(void* o_buff, uint64_t const i_bytesToRead, uint64_t* o_bytesRead /*= nullptr*/)
{
	KT_ASSERT(m_pos <= m_size);

	uint64_t const toRead = kt::Min(m_size - m_pos, i_bytesToRead);
	if (toRead)
	{
		memcpy(o_buff, (void*)(uintptr_t(m_buff) + m_pos), toRead); m_pos += toRead;
	}

	if (o_bytesRead)
	{
		*o_bytesRead = toRead;
	}

	return toRead == i_bytesToRead;
}

}