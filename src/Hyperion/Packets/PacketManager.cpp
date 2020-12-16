#include "PacketManager.hpp"

#include "PacketsIn/PacketInHandshake.h"
#include "PacketsIn/PacketInPing.h"

#include "PacketsOut/PacketOutPong.h"
#include "PacketsOut/PacketOutResponse.h"

namespace Hyperion
{
	void PacketManager::ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet)
	{
		if (!ProcessRequest(client, packet))
		{
			m_LastPacketId = static_cast<PacketInIds>(packet->GetId().GetData()[0]);
			switch (m_LastPacketId)
			{
			case Hyperion::PacketInIds::HANDSHAKE:
			{
				HP_DEBUG("HANDSHAKE");
				Ref<PacketInHandshake> handshakePacket = CreateRef<PacketInHandshake>(packet);

				HP_DEBUG("{0}, {1}, {2}, {3}", VarInt::Decode(handshakePacket->GetProtocolVersion()), handshakePacket->GetServerAddress(), handshakePacket->GetServerPort(), handshakePacket->GetState());

				client->ReadPacket();
				break;
			}
			case Hyperion::PacketInIds::PING:
			{
				HP_DEBUG("PING");
				Ref<PacketInPing> pingPacket = CreateRef<PacketInPing>(packet);

				Ref<PacketOutPong> pongPacket = CreateRef<PacketOutPong>(pingPacket->GetPayload());

				client->SendPacket(pongPacket);

				break;
			}
			default:
				break;
			}
		}
	}

	bool PacketManager::ProcessRequest(Ref<Connection> client, const Ref<Packet>& packet)
	{
		/* Request Packet */
		if (packet->GetLength().GetData()[0] != 1 || packet->GetId().GetData()[0] != 0x00)
			return false;

		switch (m_LastPacketId)
		{
		case Hyperion
		::PacketInIds::HANDSHAKE:
		
		{
			Ref<PacketOutResponse> responsePacket = CreateRef<PacketOutResponse>("{\"version\":{\"name\":\"1.16.4\",\"protocol\":754},\"players\":{\"max\":100,\"online\":5,\"sample\":[{\"name\":\"thinkofdeath\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"}]},\"description\":{\"text\":\"Hello World\",\"color\":\"yellow\"}}");
			client->SendPacket(responsePacket);
			break;
		}
		case Hyperion::PacketInIds::PING:
		{

			break;
		}
		default:
			break;
		}
		return true;
	}
}