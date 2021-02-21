#pragma once

#include <vector>

namespace VarLong
{
	std::vector<uint8_t> Encode(int64_t value);
	int64_t Decode(std::vector<uint8_t>& bytes);
};
