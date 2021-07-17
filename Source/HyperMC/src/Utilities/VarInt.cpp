/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Utilities/VarInt.hpp>

VarInt::VarInt(int32_t value)
	: m_value(value)
{
}

int32_t VarInt::value() const
{
	return m_value;
}

VarInt VarInt::decode(asio::streambuf& buffer)
{
	int32_t value = 0;
	uint32_t bit_offset = 0;
	uint8_t current_byte;
	
	size_t current_index = 0;
	const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
	
	do
	{
		current_byte = data[current_index++];
		value |= (current_byte & 0b01111111) << bit_offset;
		
		if (bit_offset == 35)
		{
			Logger::fatal("VarInt is too big!");
			return VarInt(-1);
		}
		
		bit_offset += 7;
	} while ((current_byte & 0b10000000) != 0);
	
	buffer.consume(current_index);
	
	return value;
}
