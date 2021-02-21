#include "Hyperion/HyperNetwork/PacketManager.h"

#include "Hyperion/HyperNetwork/HyperHandler/HandshakePacketHandler.h"
#include "Hyperion/HyperNetwork/HyperHandler/LoginPacketHandler.h"
#include "Hyperion/HyperNetwork/HyperHandler/PlayPacketHandler.h"
#include "Hyperion/HyperNetwork/HyperHandler/StatusPacketHandler.h"

namespace Hyperion
{
	PacketManager::PacketManager(Properties& properties, std::vector<Ref<Client>>& clients)
		: m_Properties(properties), m_Clients(clients)
	{
		m_PacketHandlers[Client::State::HANDSHAKE] = CreateScope<HandshakePacketHandler>();
		m_PacketHandlers[Client::State::STATUS] = CreateScope<StatusPacketHandler>(m_Properties);
		m_PacketHandlers[Client::State::LOGIN] = CreateScope<LoginPacketHandler>();
		m_PacketHandlers[Client::State::PLAY] = CreateScope<PlayPacketHandler>();
	}

	void PacketManager::ProcessPacket(Ref<Client> client, const Ref<Packet>& packet)
	{
		HP_INFO("Packed received: {0} -> {1}", client->GetConnectionId(), packet->GetId());
		m_PacketHandlers[client->GetCurrentState()]->ProcessPacket(client, packet);
	}
}
