#pragma once

#include <string>
#include <vector>

#include "Hyperion/HyperUtilities/Random.h"

namespace Hyperion
{
	class UUID
	{
	private:
		uint64_t m_MostSignificantBits;
		uint64_t m_LeastSignificantBits;

	public:
		UUID(uint64_t mostSignificantBits, uint64_t leastSignificantBits);
		UUID(const std::string& uuid);

		std::vector<uint8_t> GetBytes() const;

		inline uint64_t GetMostSignificantBits() const
		{
			return m_MostSignificantBits;
		}

		inline uint64_t GetLeastSignificantBits() const
		{
			return m_LeastSignificantBits;
		}

		static inline UUID RandomUUID()
		{
			Random random{};
			return { random.UInt64(), random.UInt64() };
		}

		operator std::string() const;
	};
}
