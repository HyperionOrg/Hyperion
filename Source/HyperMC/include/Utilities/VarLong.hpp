/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <asio.hpp>
#include <cstdint>

class VarLong
{
public:
	VarLong() = default;
	VarLong(int64_t value);
	
	int64_t value() const;

	static VarLong decode(asio::streambuf& buffer);
	
private:
	int64_t m_value{ 0 };
};
