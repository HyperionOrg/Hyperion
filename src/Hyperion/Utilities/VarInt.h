#pragma once

#include <cstdint>

class VarInt
{
private:
	int32_t m_Value;

public:
	VarInt(int32_t value);

	static int32_t Decode(VarInt varInt);

	operator int32_t() { return m_Value; }
};