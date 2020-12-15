#include "VarInt.h"

#include "Core.h"

VarInt::VarInt(int32_t value)
{
	do
	{
		int8_t temp = static_cast<int8_t>(value & 0b01111111);
		value = value >> 7;

		if (value != 0)
			temp |= 0b10000000;

		m_Data.push_back(temp);
	} while (value != 0);
}

int32_t VarInt::Decode(const std::vector<uint8_t>& values)
{
	uint8_t index = 0;

	int32_t numbersRead = 0;
	int32_t result = 0;
	int8_t byteRead = 0;
	do
	{
		byteRead = values[index];
		int32_t value = (byteRead & 0b01111111);
		result |= (value << (7 * numbersRead));

		HP_ASSERT(numbersRead++ <= 5, "VarInt is too big!");

		index++;
	} while ((byteRead & 0b10000000) != 0);

	return result;
}

int32_t VarInt::Decode(const VarInt& varInt)
{
	uint8_t index = 0;

	int32_t numbersRead = 0;
	int32_t result = 0;
	int8_t byteRead = 0;
	do
	{
		byteRead = varInt.m_Data[index];
		int32_t value = (byteRead & 0b01111111);
		result |= (value << (7 * numbersRead));

		HP_ASSERT(numbersRead++ <= 5, "VarInt is too big!");

		index++;
	} while ((byteRead & 0b10000000) != 0);

	return result;
}