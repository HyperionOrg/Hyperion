#pragma once

#include <vector>

class VarLong
{
private:
	std::vector<uint8_t> m_Data;

public:
	VarLong() = default;
	VarLong(uint8_t firstByte);
	VarLong(int64_t value);

	const std::vector<uint8_t>& GetData() const { return m_Data; }

	static int64_t Decode(const std::vector<uint8_t>& values);
	static int64_t Decode(const VarLong& varLong);
};