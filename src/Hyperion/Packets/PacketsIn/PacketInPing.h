#pragma once

#include "Packets/Packet.h"

namespace Hyperion
{
	class PacketInPing : public Packet
	{
	private:
		int64_t m_Payload;

	public:
		PacketInPing(const Ref<Packet> packet);

		virtual void Deserialize() override;

		int64_t GetPayload() const { return m_Payload; }
	};
}