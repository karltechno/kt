#include <catch.hpp>
#include <kt/Concurrency.h>
#include <string.h>

static void ThreadTest(kt::Thread* _t)
{
	KT_UNUSED(_t);
	std::atomic_store((std::atomic<uint32_t>*)_t->GetUserData(), 2u);
}

TEST_CASE("Thread Test 1", "")
{
	std::atomic<uint32_t> x;
	std::atomic_store_explicit(&x, 3, std::memory_order_acquire);

	kt::Thread thread;

	thread.Run(ThreadTest, &x, "hi");

	while (std::atomic_load_explicit(&x, std::memory_order_relaxed) != 2)
	{
	}

	thread.Join();
}

KT_ALIGNAS(16) struct TwoPointers
{
	uintptr_t a;
	uintptr_t b;
};

TEST_CASE("Case 128bit test", "")
{
	{
		TwoPointers a;
		TwoPointers b;

		a.a = b.a = 0x3;
		a.b = b.b = 0x4;

		CHECK(kt::AtomicCompareExchange128(&a, &b, b));
		CHECK(memcmp(&a, &b, sizeof(a)) == 0);
	}

	{
		TwoPointers a;
		TwoPointers b;

		a.a = 0x3;
		a.b = 0x4;

		b.a = 0x6;
		b.b = 0x12;

		CHECK(!kt::AtomicCompareExchange128(&a, &b, b));
		CHECK(memcmp(&a, &b, sizeof(a)) == 0);
	}
}