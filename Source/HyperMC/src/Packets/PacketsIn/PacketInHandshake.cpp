/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Core/Logger.hpp>
#include <Packets/PacketsIn/PacketInHandshake.hpp>

void PacketInHandshake::read(asio::streambuf& buffer)
{
	m_protocol_version = read_value<VarInt>(buffer);
	m_server_address = read_value<std::string>(buffer);
	m_server_port = read_value<int16_t>(buffer);
	m_next_state = static_cast<Player::State>(read_value<VarInt>(buffer).value());
	
	Logger::info("PacketInHandshake: (m_protocol_version): {}", m_protocol_version.value());
	Logger::info("PacketInHandshake: (m_server_address): {}", m_server_address);
	Logger::info("PacketInHandshake: (m_server_port): {}", m_server_port);
	Logger::info("PacketInHandshake: (m_next_state): {}", (uint8_t) m_next_state);
}

void PacketInHandshake::dispatch(Player* player) const
{
	player->set_state(m_next_state);
}
