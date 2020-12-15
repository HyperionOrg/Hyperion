#pragma once

#include <cstdint>

class VarLong
{
private:
	int64_t m_Value;

public:
	VarLong(int64_t value);

	static int64_t Decode(VarLong varInt);

	operator int64_t() { return m_Value; }
};