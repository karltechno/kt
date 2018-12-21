#pragma once
#include "kt.h"

namespace kt
{
//
//struct IAllocator;
//
//class HandleAllocator
//{
//public:
//	static uint32_t const INVALID_HANDLE = 0xFFFFFFFF;
//
//	void Init(IAllocator* _allocator, uint32_t _maxHandles, uint32_t _idxBits = 17, uint32_t _versionBits = 15);
//
//	uint32_t Alloc();
//	uint32_t GetDataIndex(uint32_t _handle);
//	bool Free(uint32_t _handle);
//	bool IsValid(uint32_t _handle);
//
//	uint32_t MaxHandles() const;
//	uint32_t AllocatedHandles() const;
//
//private:
//	IAllocator* m_allocator;
//
//	uint32_t* m_handles;
//	uint32_t m_maxHandles;
//	uint32_t m_allocatedHandles;
//
//	uint32_t m_freeFirst = INVALID_HANDLE;
//
//	uint32_t m_indexMask = 0;
//	uint32_t m_versionMask = 0;
//
//	bool m_canGrow = false;
//};

}