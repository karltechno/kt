#include "catch.hpp"

#include <kt/Handles.h>
#include <kt/Memory.h>
#include <kt/Array.h>


static void HandleAllocSparseT_Churn(kt::VersionedHandlePool<uint32_t>& _handleAlloc, uint32_t _numToChurn)
{
	using HandleType = typename kt::VersionedHandle;

	kt::Array<HandleType> handles;

	for (uint32_t i = 0; i < _numToChurn; ++i)
	{
		CHECK(_handleAlloc.NumAllocated() == i);
		uint32_t* ptr;
		HandleType handle = _handleAlloc.Alloc(ptr);
		REQUIRE(ptr);
		CHECK(_handleAlloc.IsValid(handle));
		*ptr = handle.m_packed;
		handles.PushBack(handle);
		uint32_t* lookup = _handleAlloc.Lookup(handle);
		REQUIRE(lookup);
		REQUIRE(*lookup == handle.m_packed);
	}

	CHECK(_handleAlloc.NumAllocated() == _numToChurn);
	CHECK(_handleAlloc.Capacity() >= _handleAlloc.NumAllocated());

	// Iterate
	{
		kt::Array<bool> found;
		found.Resize(_numToChurn);
		memset(found.Data(), 0, _numToChurn);

		for (uint32_t data : _handleAlloc)
		{
			REQUIRE(data < _numToChurn);
			CHECK(!found[data]);
			found[data] = true;
		}

		for (bool b : found)
		{
			CHECK(b);
		}
	}

	// Check and kill all
	for (kt::Array<HandleType>::Iterator it = handles.Begin();
		 it != handles.End();
		 /* */)
	{
		uint32_t* lookup = _handleAlloc.Lookup(*it);
		REQUIRE(lookup);
		REQUIRE(*lookup == it->m_packed);

		CHECK(_handleAlloc.IsValid(*it));
		_handleAlloc.Free(*it);
		CHECK(!_handleAlloc.IsValid(*it));
		it = handles.EraseSwap(it);
	}

	CHECK(_handleAlloc.NumAllocated() == 0);

	// create again
	for (uint32_t i = 0; i < _numToChurn; ++i)
	{
		CHECK(_handleAlloc.NumAllocated() == i);
		uint32_t* ptr;
		HandleType handle = _handleAlloc.Alloc(ptr);
		*ptr = handle.m_packed;
		handles.PushBack(handle);
		uint32_t* lookup = _handleAlloc.Lookup(handle);
		REQUIRE(lookup);
		REQUIRE(*lookup == handle.m_packed);
	}

	// Kill every other one.
	uint32_t numDeleted = 0;
	for (uint32_t i = 0; i < handles.Size(); i += 2)
	{
		HandleType const handle = handles[i];
		uint32_t* lookup = _handleAlloc.Lookup(handle);
		REQUIRE(lookup);
		REQUIRE(*lookup == handle.m_packed);
		_handleAlloc.Free(handle);
		CHECK(!_handleAlloc.Lookup(handle));
		handles.EraseSwap(i);
		numDeleted++;
	}

	CHECK(_handleAlloc.NumAllocated() == (_numToChurn - numDeleted));

	for (HandleType const handle : handles)
	{
		// Final delete
		uint32_t* lookup = _handleAlloc.Lookup(handle);
		REQUIRE(lookup);
		REQUIRE(*lookup == handle.m_packed);

		_handleAlloc.Free(handle);
		CHECK(!_handleAlloc.Lookup(handle));
	}
}

TEST_CASE("HandleAllocator_Sparse churn dynamic with data", "[handles]")
{
	kt::VersionedHandlePool<uint32_t> handleAlloc;
	handleAlloc.Init(kt::GetDefaultAllocator(), 4);

	HandleAllocSparseT_Churn(handleAlloc, 4);
	HandleAllocSparseT_Churn(handleAlloc, 8);
	HandleAllocSparseT_Churn(handleAlloc, 32);
	HandleAllocSparseT_Churn(handleAlloc, 64);
	HandleAllocSparseT_Churn(handleAlloc, 512);
	HandleAllocSparseT_Churn(handleAlloc, 2048);
}
