/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Core/Player.hpp>

Player::Player(uint64_t entity_id, asio::ip::tcp::socket* socket)
	: m_id(entity_id)
	, m_socket(socket)
{
}

Player::~Player()
{
	delete m_socket;
}

void Player::start()
{
	asio::async_read(
		*m_socket,
		m_buffer,
		[this](const std::error_code& error_code, size_t bytes_read)
		{
			return read_packet(error_code, bytes_read);
		},
		[this](const std::error_code& error_code, size_t bytes_read)
		{
			handle_read(error_code, bytes_read);
		});
}

size_t Player::read_packet(const std::error_code& error_code, size_t bytes_read)
{
	if (error_code)
	{
		Logger::fatal("Failed to read packet! {}", error_code.message());
		return 0;
	}

	if (m_buffer.size() == 0)
	{
		return m_buffer.max_size();
	}
	
	uint8_t id = asio::buffer_cast<const uint8_t*>(m_buffer.data())[0];
	m_buffer.consume(1);
	
	Logger::debug("Got packet with the id {} from entity {}", id, m_id);

	return m_buffer.max_size();
}

void Player::handle_read(const std::error_code& error_code, size_t bytes_read)
{
}
