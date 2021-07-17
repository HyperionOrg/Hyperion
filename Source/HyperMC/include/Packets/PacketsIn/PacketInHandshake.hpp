/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Packets/Packet.hpp>

class PacketInHandshake : public Packet
{
public:
	virtual void read(asio::streambuf& buffer) override;
	virtual void dispatch(Player* player) const override;

private:
	VarInt m_protocol_version;
	std::string m_server_address;
	int16_t m_server_port;
	Player::State m_next_state;
};
