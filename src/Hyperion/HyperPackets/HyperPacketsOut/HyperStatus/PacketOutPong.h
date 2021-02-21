#pragma once

#include "Hyperion/HyperNetwork/Packet.h"

namespace Hyperion
{
	class PacketOutPong : public Packet
	{
	private:
		int64_t m_Payload;

	public:
		PacketOutPong(int64_t payload);

		virtual void Serialize() override;

		int64_t GetPayload() const { return m_Payload; }
	};
}