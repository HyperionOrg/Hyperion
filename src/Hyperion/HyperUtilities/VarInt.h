#pragma once

#include <vector>

namespace VarInt
{
	std::vector<uint8_t> Encode(int32_t value);
	int32_t Decode(std::vector<uint8_t>& bytes);
};
