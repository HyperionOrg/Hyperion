/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Player.hpp>
#include <Packets/Packet.hpp>

Player::Player(uint64_t entity_id, asio::ip::tcp::socket* socket)
	: Connection(socket)
	, m_id(entity_id)
{
}

void Player::dispatch_packet(const Packet& packet)
{
	packet.dispatch(this);
}
