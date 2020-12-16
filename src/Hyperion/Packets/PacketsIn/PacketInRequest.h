#pragma once

#include "Packets/Packet.h"

namespace Hyperion
{
	class PacketInRequest : public Packet
	{
	public:
		PacketInRequest(const Ref<Packet> packet);
	};
}