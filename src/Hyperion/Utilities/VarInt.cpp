#include "VarInt.h"

#include "Core.h"

VarInt::VarInt(int32_t value)
{
	uint8_t offset = 0;
	do
	{
		int8_t temp = (int8_t)(value & 0b01111111);
		value = value >> 7;

		if (value != 0)
			temp |= 0b10000000;

		m_Value += (temp << (offset * 8));
		offset++;
	} while (value != 0);
}

int32_t VarInt::Decode(VarInt varInt)
{
	uint8_t offset = 0;

	int32_t numbersRead = 0;
	int32_t result = 0;
	int8_t byteRead = 0;
	do
	{
		byteRead = ((varInt.m_Value >> (offset * 8)) & 0xFF);
		int32_t value = (byteRead & 0b01111111);
		result |= (value << (7 * numbersRead));

		HP_ASSERT(numbersRead++ <= 5, "VarInt is too big!");

		offset++;
	} while ((byteRead & 0b10000000) != 0);

	return result;
}

int32_t VarInt::Decode(int32_t v)
{
	uint8_t offset = 0;

	int32_t numbersRead = 0;
	int32_t result = 0;
	int8_t byteRead = 0;
	do
	{
		byteRead = ((v >> (offset * 8)) & 0xFF);
		int32_t value = (byteRead & 0b01111111);
		result |= (value << (7 * numbersRead));

		HP_ASSERT(numbersRead++ <= 5, "VarInt is too big!");

		offset++;
	} while ((byteRead & 0b10000000) != 0);

	return result;
}

int32_t VarInt::Decode(uint8_t* v)
{
	uint8_t offset = 0;

	int32_t numbersRead = 0;
	int32_t result = 0;
	int8_t byteRead = 0;
	do
	{
		byteRead = v[offset];
		int32_t value = (byteRead & 0b01111111);
		result |= (value << (7 * numbersRead));

		HP_ASSERT(numbersRead++ <= 5, "VarInt is too big!");

		offset++;
	} while ((byteRead & 0b10000000) != 0);

	return result;
}