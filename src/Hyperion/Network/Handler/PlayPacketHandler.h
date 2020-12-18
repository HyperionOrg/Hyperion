#pragma once

#include "PacketHandler.h"

namespace Hyperion
{
	class PlayPacketHandler : public PacketHandler
	{
	public:
		virtual void ProcessPacket(Ref<Client> client, const Ref<Packet> packet) override;
	};
}