/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Utilities/VarLong.hpp>

VarLong::VarLong(int64_t value)
	: m_value(value)
{
}

int64_t VarLong::value() const
{
	return m_value;
}

VarLong VarLong::decode(asio::streambuf& buffer)
{
	int64_t value = 0;
	uint64_t bit_offset = 0;
	uint8_t current_byte;
	
	size_t current_index = 0;
	const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
	
	do
	{
		current_byte = data[current_index++];
		value |= (current_byte & 0b01111111) << bit_offset;

		if (bit_offset == 70)
		{
			Logger::fatal("VarLong is too big!");
			return VarLong(-1);
		}

		bit_offset += 7;
	} while ((current_byte & 0b10000000) != 0);

	buffer.consume(current_index);
	
	return value;
}
