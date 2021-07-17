/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Network/Connection.hpp>

class Player : public Connection
{
public:
	Player(uint64_t entity_id, asio::ip::tcp::socket* socket);

private:
	virtual void dispatch_packet(const Packet& packet) override;
	
private:
	uint64_t m_id{ 0 };
};
