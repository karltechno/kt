#include <catch.hpp>
#include <kt/Concurrency.h>
#include <string.h>

TEST_CASE("CAS test", "")
{ 

	{
		int16_t v = 2;
		int16_t const ret = kt::AtomicCompareAndSwap16(&v, 2u, 3u);
		CHECK(ret == 2);
		CHECK(v == 3);
	}

	{
		int32_t v = 2;
		int32_t const ret = kt::AtomicCompareAndSwap32(&v, 2u, 3u);
		CHECK(ret == 2);
		CHECK(v == 3);
	}

	{
		int64_t v = 2;
		int64_t const ret = kt::AtomicCompareAndSwap64(&v, 2u, 3u);
		CHECK(ret == 2);
		CHECK(v == 3);
	}

	{
		void* v = (void*)0x2;

		void* ret = kt::AtomicCompareAndSwapPtr(&v, (void*)0x2, (void*)0x3);
		CHECK(ret == (void*)0x2);
		CHECK(v == (void*)3);
	}
}

TEST_CASE("Fetch add test", "")
{
	{
		int8_t val = 0;
		CHECK(kt::AtomicFetchAdd8(&val, 13) == 0);
		CHECK(val == 13);
	}

	{
		int16_t val = 0;
		CHECK(kt::AtomicFetchAdd16(&val, 12) == 0);
		CHECK(val == 12);
	}

	{
		int32_t val = 0;
		CHECK(kt::AtomicFetchAdd32(&val, 13) == 0);
		CHECK(val == 13);
	}

	{
		int64_t val = 0;
		CHECK(kt::AtomicFetchAdd64(&val, 13) == 0);
		CHECK(val == 13);
	}
}

TEST_CASE("Store test", "")
{
	{
		int8_t val = 0;
		kt::AtomicStore8(&val, 13);
		CHECK(val == 13);
	}

	{
		int16_t val = 0;
		kt::AtomicStore16(&val, 13);
		CHECK(val == 13);
	}

	{
		int32_t val = 0;
		kt::AtomicStore32(&val, 13);
		CHECK(val == 13);
	}

	{
		int64_t val = 0;
		kt::AtomicStore64(&val, 13);
		CHECK(val == 13);
	}
}

TEST_CASE("Load test", "")
{
	{
		int8_t val = 34;
		CHECK(kt::AtomicLoad8(&val) == 34);
	}

	{
		int16_t val = 34;
		CHECK(kt::AtomicLoad16(&val) == 34);
	}

	{
		int32_t val = 34;
		CHECK(kt::AtomicLoad32(&val) == 34);
	}

	{
		int64_t val = 34;
		CHECK(kt::AtomicLoad64(&val) == 34);
	}
}

static void ThreadTest(kt::Thread* _t)
{
	KT_UNUSED(_t);
	kt::AtomicStore32((int32_t*)_t->GetUserData(), 2u);
}

TEST_CASE("Thread Test 1", "")
{
	uint32_t x = 3;

	kt::Thread thread;

	thread.Run(ThreadTest, &x, "hi");

	while (kt::AtomicLoad32((int32_t*)&x) != 2)
	{
	}

	thread.Join();
}