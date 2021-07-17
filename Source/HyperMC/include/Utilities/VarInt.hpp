/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <asio.hpp>
#include <cstdint>

class VarInt
{
public:
	VarInt() = default;
	VarInt(int32_t value);
	
	int32_t value() const;
	
	static VarInt decode(asio::streambuf& buffer);

private:
	int32_t m_value{ 0 };
};
