#pragma once

#include <vector>

class VarInt
{
private:
	std::vector<uint8_t> m_Data;

public:
	VarInt() = default;
	VarInt(uint8_t firstByte = 0);
	VarInt(int32_t value = 0);

	const std::vector<uint8_t>& GetData() const { return m_Data; }

	static int32_t Decode(const std::vector<uint8_t>& values);
	static int32_t Decode(const VarInt& varInt);
};