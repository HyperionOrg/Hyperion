#include "Hyperion/HyperNetwork/HyperHandler/LoginPacketHandler.h"

#include "Hyperion/HyperPackets/HyperPacketsIn/HyperLogin/PacketInLogin.h"
#include "Hyperion/HyperPackets/HyperPacketsOut/HyperLogin/PacketOutLoginSuccess.h"
#include "Hyperion/HyperPackets/HyperPacketsOut/HyperPlay/PacketOutJoinGame.h"

namespace Hyperion
{
	void LoginPacketHandler::ProcessPacket(Ref<Client> client, const Ref<Packet> packet)
	{
		switch (packet->GetId())
		{
		case 0x00:
		{
			Ref<PacketInLogin> packetLogin = ToPacket<PacketInLogin>(packet);

			client->m_CurrentState = Client::State::PLAY;

			Ref<PacketOutLoginSuccess> packetLoginSuccess = CreateRef<PacketOutLoginSuccess>(UUID::RandomUUID(), packetLogin->GetUsername());
			SendPacket(client, packetLoginSuccess);

			Ref<PacketOutJoinGame> packetJoinGame = CreateRef<PacketOutJoinGame>();
			//SendPacket(client, packetJoinGame);
			break;
		}
		default:
			break;
		}
	}
}