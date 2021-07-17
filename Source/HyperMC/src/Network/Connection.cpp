/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Network/Connection.hpp>
#include <Packets/Packet.hpp>

Connection::Connection(asio::ip::tcp::socket* socket)
	: m_socket(socket)
{
}

Connection::~Connection()
{
	delete m_socket;
}

void Connection::start_reading()
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

void Connection::set_state(Connection::State state)
{
	m_state = state;
}

Connection::State Connection::state() const
{
	return m_state;
}

size_t Connection::read_packet(const std::error_code& error_code, size_t bytes_read)
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

	if (m_packet == nullptr)
	{
		VarInt::decode(m_buffer);
		VarInt packet_id = VarInt::decode(m_buffer);

		Logger::info("Packet {:#04x}", packet_id.value());
		m_packet = Packet::from_id(m_state, static_cast<uint64_t>(packet_id.value()));
	}

	m_packet->read(m_buffer);
	
	return 0;
}

void Connection::handle_read(const std::error_code& error_code, size_t bytes_read)
{
	if (error_code && m_socket->is_open())
	{
		Logger::fatal("Failed to read complete packet! {}", error_code.message());
		return;
	}

	dispatch_packet(*m_packet);
	delete m_packet;
	
	m_packet = nullptr;

	start_reading();
}
