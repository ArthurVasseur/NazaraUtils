// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_BITSET_HPP
#define NAZARAUTILS_BITSET_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/MathUtils.hpp>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace Nz
{
	template<typename Block = UInt32, class Allocator = std::allocator<Block>>
	class Bitset
	{
		static_assert(std::is_integral<Block>::value && std::is_unsigned<Block>::value, "Block must be a unsigned integral type");

		public:
			class Bit;
			class BitIterator;
			using PointerSequence = std::pair<const void*, std::size_t>; //< Start pointer, bit offset
			struct bits_const_iter_tag;

			Bitset();
			explicit Bitset(std::size_t bitCount, bool val);
			explicit Bitset(const char* bits);
			Bitset(const char* bits, std::size_t bitCount);
			Bitset(const Bitset& bitset) = default;
			explicit Bitset(const std::string_view& bits);
			explicit Bitset(const std::string& bits);
			template<typename T> Bitset(T value);
			Bitset(Bitset&& bitset) noexcept = default;
			~Bitset() noexcept = default;

			template<typename T> void AppendBits(T bits, std::size_t bitCount);

			void Clear() noexcept;
			std::size_t Count() const;
			void Flip();

			std::size_t FindFirst() const;
			std::size_t FindNext(std::size_t bit) const;

			Block GetBlock(std::size_t i) const;
			std::size_t GetBlockCount() const;
			std::size_t GetCapacity() const;
			std::size_t GetSize() const;

			void PerformsAND(const Bitset& a, const Bitset& b);
			void PerformsNOT(const Bitset& a);
			void PerformsOR(const Bitset& a, const Bitset& b);
			void PerformsXOR(const Bitset& a, const Bitset& b);

			bool Intersects(const Bitset& bitset) const;

			constexpr bits_const_iter_tag IterBits() const noexcept;

			void Reserve(std::size_t bitCount);
			void Resize(std::size_t bitCount, bool defaultVal = false);

			void Reset();
			void Reset(std::size_t bit);

			void Reverse();

			void Set(bool val = true);
			void Set(std::size_t bit, bool val = true);
			void SetBlock(std::size_t i, Block block);

			void ShiftLeft(std::size_t pos);
			void ShiftRight(std::size_t pos);

			void Swap(Bitset& bitset) noexcept;

			bool Test(std::size_t bit) const;
			bool TestAll() const;
			bool TestAny() const;
			bool TestNone() const;

			template<typename T> T To() const;
			std::string ToString() const;

			void UnboundedReset(std::size_t bit);
			void UnboundedSet(std::size_t bit, bool val = true);
			bool UnboundedTest(std::size_t bit) const;

			PointerSequence Write(const void* ptr, std::size_t bitCount);
			PointerSequence Write(const PointerSequence& sequence, std::size_t bitCount);

			Bit operator[](std::size_t index);
			bool operator[](std::size_t index) const;

			Bitset operator~() const;

			Bitset& operator=(const Bitset& bitset) = default;
			Bitset& operator=(const std::string_view& bits);
			template<typename T> Bitset& operator=(T value);
			Bitset& operator=(Bitset&& bitset) noexcept = default;

			Bitset operator<<(std::size_t pos) const;
			Bitset& operator<<=(std::size_t pos);

			Bitset operator>>(std::size_t pos) const;
			Bitset& operator>>=(std::size_t pos);

			Bitset& operator&=(const Bitset& bitset);
			Bitset& operator|=(const Bitset& bitset);
			Bitset& operator^=(const Bitset& bitset);

			static constexpr Block fullBitMask = std::numeric_limits<Block>::max();
			static constexpr std::size_t bitsPerBlock = BitCount<Block>();
			static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

			static Bitset FromPointer(const void* ptr, std::size_t bitCount, PointerSequence* sequence = nullptr);

			struct bits_const_iter_tag
			{
				constexpr BitIterator begin() const noexcept;
				constexpr BitIterator end() const noexcept;

				const Bitset& bitsetRef;
			};

		private:
			std::size_t FindFirstFrom(std::size_t blockIndex) const;
			Block GetLastBlockMask() const;
			void ResetExtraBits();

			static std::size_t ComputeBlockCount(std::size_t bitCount);
			static std::size_t GetBitIndex(std::size_t bit);
			static std::size_t GetBlockIndex(std::size_t bit);

			std::vector<Block, Allocator> m_blocks;
			std::size_t m_bitCount;
	};

	template<typename Block, class Allocator>
	class Bitset<Block, Allocator>::Bit
	{
		friend Bitset<Block, Allocator>;

		public:
			Bit(const Bit& bit) = default;

			Bit& Flip();
			Bit& Reset();
			Bit& Set(bool val = true);
			bool Test() const;

			template<bool BadCall = true>
			void* operator&() const;

			operator bool() const;
			Bit& operator=(bool val);
			Bit& operator=(const Bit& bit);

			Bit& operator|=(bool val);
			Bit& operator&=(bool val);
			Bit& operator^=(bool val);
			Bit& operator-=(bool val);

		private:
			Bit(Block& block, Block mask) :
			m_block(block),
			m_mask(mask)
			{
			}

			Block& m_block;
			Block m_mask;
	};
	
	template<typename Block, class Allocator>
	class Bitset<Block, Allocator>::BitIterator
	{
		friend Bitset;

		public:
			using iterator_category = std::input_iterator_tag;
			using value_type = std::size_t;

			constexpr BitIterator(const BitIterator&) = default;
			constexpr BitIterator(BitIterator&&) noexcept = default;

			constexpr BitIterator& operator=(const BitIterator&) = default;
			constexpr BitIterator& operator=(BitIterator&&) noexcept = default;

			constexpr BitIterator operator++(int);
			constexpr BitIterator& operator++();

			constexpr bool operator==(const BitIterator& rhs) const;
			constexpr bool operator!=(const BitIterator& rhs) const;
			constexpr value_type operator*() const;

		private:
			constexpr BitIterator(bits_const_iter_tag bitsetTag, std::size_t bitIndex);

			std::size_t m_bitIndex;
			const Bitset* m_owner;
	};

	template<typename Block, class Allocator>
	std::ostream& operator<<(std::ostream& out, const Bitset<Block, Allocator>& bitset);

	template<typename Block, class Allocator>
	bool operator==(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	bool operator!=(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	bool operator<(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	bool operator<=(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	bool operator>(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	bool operator>=(const Bitset<Block, Allocator>& lhs, const Nz::Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	Bitset<Block, Allocator> operator&(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	Bitset<Block, Allocator> operator|(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs);

	template<typename Block, class Allocator>
	Bitset<Block, Allocator> operator^(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs);
}

namespace std
{
	template<typename Block, class Allocator>
	void swap(Nz::Bitset<Block, Allocator>& lhs, Nz::Bitset<Block, Allocator>& rhs) noexcept;
}

#include <NazaraUtils/Bitset.inl>

#endif // NAZARAUTILS_BITSET_HPP
