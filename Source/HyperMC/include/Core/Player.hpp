/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <asio.hpp>

class Player
{
public:
	Player(uint64_t entity_id, asio::ip::tcp::socket* socket);
	~Player();
	
	void start();

private:
	size_t read_packet(const std::error_code& error_code, size_t bytes_read);
	void handle_read(const std::error_code& error_code, size_t bytes_read);

private:
	uint64_t m_id{ 0 };
	
	asio::ip::tcp::socket* m_socket{ nullptr };
	asio::streambuf m_buffer;
};
