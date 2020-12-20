#include "HandshakePacketHandler.h"

#include "Packets/PacketsIn/Handshake/PacketInHandshake.h"

namespace Hyperion
{
	void HandshakePacketHandler::ProcessPacket(Ref<Client> client, const Ref<Packet> packet)
	{
		Ref<PacketInHandshake> packetHandshake = ToPacket<PacketInHandshake>(packet);

		client->m_ProtocolVersion = packetHandshake->GetProtocolVersion();
		client->m_CurrentState = static_cast<Client::State>(packetHandshake->GetNextState());
	}
}