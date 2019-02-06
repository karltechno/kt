#include <kt/kt.h>
#include <kt/Memory.h>
#include "catch.hpp"

TEST_CASE("Operator new/delete", "")
{
	struct MyDummyAllocator : kt::IAllocator
	{
		uint32_t allocs = 0;
		uint32_t frees = 0;
		
		kt::CrtAllocator allocator;

		virtual void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN)
		{
			++allocs;
			return allocator.Alloc(_sz, _align);
		}

		virtual void* ReAlloc(void* _ptr, size_t const _sz)
		{
			return allocator.ReAlloc(_ptr, _sz);
		}

		virtual void Free(void* _ptr)
		{
			++frees;
			return allocator.Free(_ptr);
		}
	};

	MyDummyAllocator allocator;
	kt::IAllocator* oldAllocator = kt::GetDefaultAllocator();
	KT_SCOPE_EXIT(kt::SetDefaultAllocator(oldAllocator));

	kt::SetDefaultAllocator(&allocator);

	uint32_t expectedAllocs = 0;

	int32_t* p = new int32_t;
	++expectedAllocs;

	uint32_t* p2 = new uint32_t;
	++expectedAllocs;

	uint32_t* p3 = new uint32_t[128];
	++expectedAllocs;

	int32_t* p4 = new int32_t[128];
	++expectedAllocs;

	CHECK(allocator.allocs == expectedAllocs);

	delete p;
	delete p2;
	delete[] p3;
	delete[] p4;

	CHECK(allocator.frees == expectedAllocs);
}
