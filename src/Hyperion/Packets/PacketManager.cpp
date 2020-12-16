#include "PacketManager.hpp"

#include "PacketsIn/PacketInHandshake.h"

namespace Hyperion
{
	void PacketManager::ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet)
	{
		PacketInIds id = static_cast<PacketInIds>(packet->GetId().GetData()[0]);

		switch (id)
		{
		case Hyperion::PacketInIds::HANDSHAKE:
		{
			PacketInHandshake handshakePacket(packet);

			HP_DEBUG("HANDSHAKE");
			HP_DEBUG("{0}, {1}, {2}, {3}", VarInt::Decode(handshakePacket.GetProtocolVersion()), handshakePacket.GetServerAddress(), handshakePacket.GetServerPort(), handshakePacket.GetState());
			break;
		}
		case Hyperion::PacketInIds::PING:
		{
			HP_DEBUG("PING");
			break;
		}
		default:
			break;
		}
	}
}