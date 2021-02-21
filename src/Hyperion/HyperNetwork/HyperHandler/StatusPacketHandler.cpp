#include "Hyperion/HyperNetwork/HyperHandler/StatusPacketHandler.h"

#include "Hyperion/HyperObjects/ServerListPing.h"

#include "Hyperion/HyperPackets/HyperPacketsIn/HyperStatus/PacketInPing.h"
#include "Hyperion/HyperPackets/HyperPacketsIn/HyperStatus/PacketInRequest.h"

#include "Hyperion/HyperPackets/HyperPacketsOut/HyperStatus/PacketOutPong.h"
#include "Hyperion/HyperPackets/HyperPacketsOut/HyperStatus/PacketOutResponse.h"

namespace Hyperion
{
	StatusPacketHandler::StatusPacketHandler(Properties& properties)
		: m_Properties(properties)
	{
	}

	void StatusPacketHandler::ProcessPacket(Ref<Client> client, const Ref<Packet> packet)
	{
		switch (packet->GetId())
		{
		case 0x00:
		{
			Ref<PacketInRequest> packetRequest = ToPacket<PacketInRequest>(packet);

			std::optional<int32_t> maxPlayers = m_Properties.GetInt("max-players");
			std::optional<std::string> motd = m_Properties.GetString("motd");

			ServerListPingInfo serverListPing{};
			serverListPing.Version.Name = "1.16.5";
			serverListPing.Version.ProtocolVersion = 754;
			serverListPing.Players.Max = maxPlayers.value_or(-1);
			serverListPing.Players.Online = 0;
			serverListPing.Players.Sample = { { "Test User", "e9013c2f-da01-425f-a48b-516f55e94386" } };
			serverListPing.Motd = motd.value_or("");

			Ref<PacketOutResponse> responsePacket = CreateRef<PacketOutResponse>(static_cast<nlohmann::json>(serverListPing).dump());
			SendPacket(client, responsePacket);
			break;
		}
		case 0x01:
		{
			Ref<PacketInPing> packetPing = ToPacket<PacketInPing>(packet);

			Ref<PacketOutPong> packetPong = CreateRef<PacketOutPong>(packetPing->GetPayload());
			SendPacket(client, packetPong);
			break;
		}
		default:
			break;
		}
	}
}
