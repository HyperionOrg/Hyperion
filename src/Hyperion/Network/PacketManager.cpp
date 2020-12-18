#include "PacketManager.h"

#include "Handler/HandshakePacketHandler.h"
#include "Handler/LoginPacketHandler.h"
#include "Handler/PlayPacketHandler.h"
#include "Handler/StatusPacketHandler.h"

namespace Hyperion
{
	PacketManager::PacketManager(std::deque<Ref<Client>>& clients, std::function<void(Ref<Client>)> disconnectFunction)
		: m_Clients(clients), m_DisconnectFunction(disconnectFunction)
	{
		m_PacketHandlers[Client::State::HANDSHAKE] = CreateScope<HandshakePacketHandler>();
		m_PacketHandlers[Client::State::STATUS] = CreateScope<StatusPacketHandler>();
		m_PacketHandlers[Client::State::LOGIN] = CreateScope<LoginPacketHandler>();
		m_PacketHandlers[Client::State::PLAY] = CreateScope<PlayPacketHandler>();
	}

	void PacketManager::ProcessPacket(Ref<Client> client, const Ref<Packet>& packet)
	{
		m_PacketHandlers[client->GetCurrentState()]->ProcessPacket(client, packet);
	}
}