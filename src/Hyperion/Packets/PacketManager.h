#pragma once

#include <functional>
#include <queue>

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

		std::deque<Ref<Connection>>& m_Connections;
		std::function<void(Ref<Connection>)> m_DisconnectFunction;

	public:
		PacketManager(std::deque<Ref<Connection>>& connections, std::function<void(Ref<Connection>)> disconnectFunction);

		void ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet);

		void SendPacketToAllClients(const Ref<Packet>& packet);
		void SendPacketToClient(Ref<Connection> client, const Ref<Packet>& packet);
		void SendPacketToClients(std::vector<Ref<Connection>>& clients, const Ref<Packet>& packet);

		void KillInvalidClient(Ref<Connection> client);

	private:
		bool ProcessRequest(Ref<Connection> client, const Ref<Packet>& packet);
	};
}