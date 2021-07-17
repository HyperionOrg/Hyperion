/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Core/Player.hpp>
#include <Utilities/VarInt.hpp>
#include <Utilities/VarLong.hpp>
#include <asio.hpp>
#include <string>

class Packet
{
public:
	virtual ~Packet() = default;

	virtual void read(asio::streambuf& buffer) = 0;
	virtual void dispatch(Player* player) const = 0;

	static Packet* from_id(Player::State state, uint64_t id);

protected:
	template <typename T>
	T read_value(asio::streambuf& buffer) = delete;

	template <>
	int8_t read_value<int8_t>(asio::streambuf& buffer)
	{
		const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
		
		auto value = static_cast<int8_t>(data[0]);
		buffer.consume(sizeof(int8_t));
		
		return value;
	}

	template <>
	int16_t read_value<int16_t>(asio::streambuf& buffer)
	{
		const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
		
		int16_t value = 0;

		for (size_t i = 0; i < sizeof(int16_t); i++)
		{
			value |= static_cast<int16_t>((data[i] & 0xFF) << ((sizeof(int16_t) - i - 1) * 8));
		}
		
		buffer.consume(sizeof(int16_t));
		
		return value;
	}

	template <>
	int32_t read_value<int32_t>(asio::streambuf& buffer)
	{
		const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
		
		int32_t value = 0;

		for (size_t i = 0; i < sizeof(int32_t); i++)
		{
			value |= static_cast<int32_t>((data[i] & 0xFF) << ((sizeof(int32_t) - i - 1) * 8));
		}
		
		buffer.consume(sizeof(int32_t));
		
		return value;
	}

	template <>
	int64_t read_value<int64_t>(asio::streambuf& buffer)
	{
		const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());
		
		int64_t value = 0;

		for (size_t i = 0; i < sizeof(int64_t); i++)
		{
			value |= static_cast<int64_t>((data[i] & 0xFF) << ((sizeof(int64_t) - i - 1) * 8));
		}
		
		buffer.consume(sizeof(int64_t));
		
		return value;
	}

	template <>
	std::string read_value<std::string>(asio::streambuf& buffer)
	{
		auto length = static_cast<size_t>(read_value<VarInt>(buffer).value());

		const auto* data = asio::buffer_cast<const uint8_t*>(buffer.data());

		std::string string;
		for (size_t i = 0; i < length; i++)
		{
			string += static_cast<char>(data[i]);
		}

		buffer.consume(length);

		return string;
	}

	template <>
	VarInt read_value<VarInt>(asio::streambuf& buffer)
	{
		return VarInt::decode(buffer);
	}

	template <>
	VarLong read_value<VarLong>(asio::streambuf& buffer)
	{
		return VarLong::decode(buffer);
	}

private:
	static Packet* from_handshaking(uint64_t id);
	static Packet* from_status(uint64_t id);
	static Packet* from_login(uint64_t id);
	static Packet* from_play(uint64_t id);
};
