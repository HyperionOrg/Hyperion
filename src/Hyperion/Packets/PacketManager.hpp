#pragma once

#include "Core.h"
#include "Packet.h"
#include "Network/Connection.h"

namespace Hyperion
{
	/* Client -> Server */
	enum class PacketInIds : uint8_t
	{
		HANDSHAKE = 0x00,
		PING = 0x01
	};

	/* Server -> Client */
	enum class PacketOutIds : uint8_t
	{
		PONG = 0x01
	};

	class PacketManager
	{
	private:
		PacketInIds m_LastPacketId;

	public:
		void ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet);

	private:
		bool ProcessRequest(Ref<Connection> client, const Ref<Packet>& packet);
	};
}