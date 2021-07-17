/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Network/Server.hpp>
#include <iostream>

Server::Server(uint32_t port)
	: m_port(port)
	, m_endpoint(asio::ip::tcp::v4(), m_port)
	, m_acceptor(m_io_service, m_endpoint)
{
	Logger::info("Starting minecraft server version 1.17.1");

	accept_next_connection();
}

void Server::accept_next_connection()
{
	auto* socket = new asio::ip::tcp::socket(m_io_service);

	m_acceptor.async_accept(
		*socket,
		[socket, this](const std::error_code& error_code)
		{
			handle_connection(error_code, socket);
		});
}

void Server::handle_connection(const std::error_code& error_code, asio::ip::tcp::socket* socket)
{
	if (error_code)
	{
		Logger::fatal("Failed to accept the connection! {}", error_code.message());
		delete socket;

		m_running = false;

		return;
	}

	socket->remote_endpoint().address();

	Logger::info("Connection successfully accepted! ({})", socket->remote_endpoint().address().to_string());

	auto* player = new Player(m_entity_id++, socket);
	player->start_reading();

	m_players.push_back(player);

	accept_next_connection();
}

void Server::run()
{
	while (m_running)
	{
		m_io_service.run();
	}
}
