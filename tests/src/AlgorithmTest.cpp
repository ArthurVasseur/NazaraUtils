#include <NazaraUtils/Algorithm.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <map>

// Constexprness test
#ifdef NAZARA_HAS_CONSTEVAL
static_assert(Nz::CountBits(65) == 2);
static_assert(Nz::FindFirstBit(0) == 0);
static_assert(Nz::FindFirstBit(0b00110101) == 1);
static_assert(Nz::FindFirstBit(0b00110100) == 3);
#endif

static_assert(Nz::SetBit(0b00110001, 1) == 0b00110011);
static_assert(Nz::TestBit(0b00110001, 0));
static_assert(!Nz::TestBit(0b00110001, 1));

template<typename T>
void TestCountBits()
{
	T value = 0;
	CHECK(Nz::CountBits(value) == 0);
	CHECK(Nz::CountBits(std::numeric_limits<T>::max()) == Nz::BitCount<T>());
	CHECK(Nz::CountBits(std::numeric_limits<T>::max() / 2) == Nz::BitCount<T>() - 1);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::CountBits(value) == 1);
	}
}

template<typename T>
void TestFindFirstBit()
{
	T value = 0;
	CHECK(Nz::FindFirstBit(value) == 0);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::FindFirstBit(value) == i + 1);
	}
}

SCENARIO("Algorithm", "[Algorithm]")
{
	WHEN("Testing SafeCast")
	{
		// Catch2 offers no way to catch an assertion failure
		CHECK(Nz::SafeCast<Nz::UInt64>(3.0) == 3);
		CHECK(Nz::SafeCast<Nz::UInt64>(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
		CHECK(Nz::SafeCast<int>(Nz::UInt64(42)) == 42);
	}

	WHEN("Testing Retrieve")
	{
		using namespace std::literals;

		std::map<std::string, std::size_t, std::less<>> strMap = {
			{
				{ "Foo", 1 },
				{ "Bar", 2 },
				{ "Baz", 3 },
			}
		};

		CHECK(Nz::Retrieve(strMap, "Foo") == 1);
		CHECK(Nz::Retrieve(strMap, "Bar") == 2);
		CHECK(Nz::Retrieve(strMap, "Baz") == 3);

		CHECK(Nz::Retrieve(strMap, "Foo"sv) == 1);
		CHECK(Nz::Retrieve(strMap, "Bar"sv) == 2);
		CHECK(Nz::Retrieve(strMap, "Baz"sv) == 3);

		CHECK(Nz::Retrieve(strMap, "Foo"s) == 1);
		CHECK(Nz::Retrieve(strMap, "Bar"s) == 2);
		CHECK(Nz::Retrieve(strMap, "Baz"s) == 3);
	}

	WHEN("Testing CountBit")
	{
		TestCountBits<Nz::UInt8>();
		TestCountBits<Nz::UInt16>();
		TestCountBits<Nz::UInt32>();
		TestCountBits<Nz::UInt64>();
	}

	WHEN("Testing FindFirstBit")
	{
		TestFindFirstBit<Nz::UInt8>();
		TestFindFirstBit<Nz::UInt16>();
		TestFindFirstBit<Nz::UInt32>();
		TestFindFirstBit<Nz::UInt64>();
	}

	WHEN("Testing Mod")
	{
#ifdef NAZARA_HAS_CONSTEVAL
		static_assert(Nz::Mod(3, 2) == 1);
		static_assert(Nz::NumberEquals(Nz::Mod(3.f, 2.f), 1.f, 0.00001f));
#endif

		CHECK(Nz::Mod(3, 2) == 1);
		CHECK(Nz::Mod(3.f, 2.f) == Catch::Approx(1.f));
	}

	WHEN("Testing ByteSwap")
	{
#ifdef NAZARA_HAS_CONSTEVAL
		static_assert(Nz::ByteSwap(std::int16_t(0xABCD)) == std::int16_t(0xCDAB));
		static_assert(Nz::ByteSwap(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
		static_assert(Nz::ByteSwap(std::int32_t(0xABCDEF01)) == std::int32_t(0x01EFCDAB));
		static_assert(Nz::ByteSwap(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
		static_assert(Nz::ByteSwap(std::int64_t(0xABCDEF0102030405)) == std::int64_t(0x0504030201EFCDAB));
		static_assert(Nz::ByteSwap(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));
#endif

		CHECK(Nz::ByteSwap(std::int16_t(0xABCD)) == std::int16_t(0xCDAB));
		CHECK(Nz::ByteSwap(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
		CHECK(Nz::ByteSwap(std::int32_t(0xABCDEF01)) == std::int32_t(0x01EFCDAB));
		CHECK(Nz::ByteSwap(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
		CHECK(Nz::ByteSwap(std::int64_t(0xABCDEF0102030405)) == std::int64_t(0x0504030201EFCDAB));
		CHECK(Nz::ByteSwap(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));

#ifdef NAZARA_HAS_CONSTEXPR_BITCAST
		static_assert(Nz::BitCast<std::uint32_t>(Nz::ByteSwap(Nz::BitCast<float>(0xABCDEF01))) == std::uint32_t(0x01EFCDAB));
		static_assert(Nz::BitCast<std::uint64_t>(Nz::ByteSwap(Nz::BitCast<double>(0xABCDEF0102030405))) == std::uint64_t(0x0504030201EFCDAB));
#endif

		CHECK(Nz::BitCast<std::uint32_t>(Nz::ByteSwap(Nz::BitCast<float>(0xABCDEF01))) == std::uint32_t(0x01EFCDAB));
		CHECK(Nz::BitCast<std::uint64_t>(Nz::ByteSwap(Nz::BitCast<double>(0xABCDEF0102030405))) == std::uint64_t(0x0504030201EFCDAB));
	}
}
