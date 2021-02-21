#include "Hyperion/HyperNetwork/HyperHandler/HandshakePacketHandler.h"

#include "Hyperion/HyperPackets/HyperPacketsIn/HyperHandshake/PacketInHandshake.h"

namespace Hyperion
{
	void HandshakePacketHandler::ProcessPacket(Ref<Client> client, const Ref<Packet> packet)
	{
		Ref<PacketInHandshake> packetHandshake = ToPacket<PacketInHandshake>(packet);

		client->m_ProtocolVersion = packetHandshake->GetProtocolVersion();
		client->m_CurrentState = static_cast<Client::State>(packetHandshake->GetNextState());
	}
}