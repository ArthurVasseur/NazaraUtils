// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/Algorithm.hpp>

namespace Nz
{
	template<typename T>
	constexpr T BigEndianToHost(T value)
	{
#if defined(NAZARA_BIG_ENDIAN)
		return value;
#elif defined(NAZARA_LITTLE_ENDIAN)
		return ByteSwap(value);
#endif
	}

	template<typename T>
	constexpr T LittleEndianToHost(T value)
	{
#if defined(NAZARA_BIG_ENDIAN)
		return ByteSwap(value);
#elif defined(NAZARA_LITTLE_ENDIAN)
		return value;
#endif
	}

	template<typename T>
	constexpr T HostToBigEndian(T value)
	{
		return BigEndianToHost(value);
	}

	template<typename T>
	constexpr T HostToLittleEndian(T value)
	{
		return LittleEndianToHost(value);
	}
}

