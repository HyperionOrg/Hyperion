#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "Hyperion/Core.h"
#include "Hyperion/HyperNetwork/Client.h"
#include "Hyperion/HyperNetwork/Packet.h"
#include "Hyperion/HyperNetwork/HyperHandler/PacketHandler.h"
#include "Hyperion/HyperUtilities/Properties.h"

namespace Hyperion
{
	class PacketManager
	{
	private:
		Properties& m_Properties;
		std::deque<Ref<Client>>& m_Clients;

		std::unordered_map<Client::State, Scope<PacketHandler>> m_PacketHandlers;

	public:
		PacketManager(Properties& properties, std::deque<Ref<Client>>& clients);

		void ProcessPacket(Ref<Client> client, const Ref<Packet>& packet);
	};
}
