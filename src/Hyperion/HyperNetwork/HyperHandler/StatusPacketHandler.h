#pragma once

#include "Hyperion/HyperNetwork/HyperHandler/PacketHandler.h"
#include "Hyperion/HyperUtilities/Properties.h"

namespace Hyperion
{
	class StatusPacketHandler : public PacketHandler
	{
	private:
		Properties& m_Properties;

	public:
		StatusPacketHandler(Properties& properties);

		virtual void ProcessPacket(Ref<Client> client, const Ref<Packet> packet) override;
	};
}