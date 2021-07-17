/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <asio.hpp>

class Packet;

class Connection
{
public:
	enum class State
	{
		Handshaking,
		Status,
		Login,
		Play
	};

public:
	explicit Connection(asio::ip::tcp::socket* socket);
	virtual ~Connection();
	
	void start_reading();
	
	void set_state(State state);
	State state() const;
	
protected:
	virtual void dispatch_packet(const Packet& packet) = 0;
	
private:
	size_t read_packet(const std::error_code& error_code, size_t bytes_read);
	void handle_read(const std::error_code& error_code, size_t bytes_read);
	
private:
	asio::ip::tcp::socket* m_socket{ nullptr };
	asio::streambuf m_buffer;
	
	Packet* m_packet{ nullptr };
	State m_state{ State::Handshaking };
};
