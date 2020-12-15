#include "PacketManager.hpp"

namespace Hyperion
{
	void PacketManager::ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet)
	{
		PacketInIds id = static_cast<PacketInIds>(packet->GetId().GetData()[0]);

		switch (id)
		{
		case Hyperion::PacketInIds::HANDSHAKE:
			break;
		case Hyperion::PacketInIds::PING:
			break;
		default:
			break;
		}
	}
}