#pragma once

#include <functional>
#include <queue>
#include <unordered_map>

#include "Client.h"
#include "Core.h"
#include "Packet.h"
#include "Handler/PacketHandler.h"

namespace Hyperion
{
	class PacketManager
	{
	private:
		std::deque<Ref<Client>>& m_Clients;
		std::function<void(Ref<Client>)> m_DisconnectFunction;

		std::unordered_map<Client::State, Scope<PacketHandler>> m_PacketHandlers;

	public:
		PacketManager(std::deque<Ref<Client>>& clients, std::function<void(Ref<Client>)> disconnectFunction);

		void ProcessPacket(Ref<Client> client, const Ref<Packet>& packet);
	};
}