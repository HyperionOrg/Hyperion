/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Packets/Packet.hpp>
#include <Packets/PacketsIn/PacketInHandshake.hpp>

Packet* Packet::from_id(Player::State state, uint64_t id)
{
	switch (state)
	{
	case Player::State::Handshaking:
		return from_handshaking(id);
	case Player::State::Status:
		return from_status(id);
	case Player::State::Login:
		return from_login(id);
	case Player::State::Play:
		return from_play(id);
	default:
		break;
	}

	return nullptr;
}

Packet* Packet::from_handshaking(uint64_t id)
{
	switch (id)
	{
	case 0x00:
		return new PacketInHandshake();
	default:
		break;
	}
	return nullptr;
}

Packet* Packet::from_status(uint64_t id)
{
	switch (id)
	{
	case 0x00:
		break;
	default:
		break;
	}
	return nullptr;
}

Packet* Packet::from_login(uint64_t id)
{
	switch (id)
	{
	case 0x00:
		break;
	default:
		break;
	}
	return nullptr;
}

Packet* Packet::from_play(uint64_t id)
{
	switch (id)
	{
	case 0x00:
		break;
	default:
		break;
	}
	return nullptr;
}
