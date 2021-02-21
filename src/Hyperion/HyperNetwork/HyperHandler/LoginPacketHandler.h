#pragma once

#include "Hyperion/HyperNetwork/HyperHandler/PacketHandler.h"

namespace Hyperion
{
	class LoginPacketHandler : public PacketHandler
	{
	public:
		virtual void ProcessPacket(Ref<Client> client, const Ref<Packet> packet) override;
	};
}