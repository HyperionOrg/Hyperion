#pragma once

#include <string>
#include <vector>

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

		uint64_t GetMostSignificantBits();
		uint64_t GetLeastSignificantBits();

		std::vector<uint8_t> GetBytes();

		static UUID RandomUUID();

		operator std::string() const;
	};
}