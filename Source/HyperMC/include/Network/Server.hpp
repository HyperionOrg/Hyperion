/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Core/Player.hpp>
#include <asio.hpp>
#include <vector>

class Server
{
public:
	explicit Server(uint32_t port);

	void run();

private:
	void accept_next_connection();
	void handle_connection(const std::error_code& error_code, asio::ip::tcp::socket* socket);

private:
	uint32_t m_port{ 25565 };
	bool m_running{ true };
	
	uint64_t m_entity_id{ 0 };
	std::vector<Player*> m_players{};

	asio::io_service m_io_service{};
	asio::ip::tcp::endpoint m_endpoint;
	asio::ip::tcp::acceptor m_acceptor;
};
