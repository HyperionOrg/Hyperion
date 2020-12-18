#pragma once

#include "Network/Packet.h"

namespace Hyperion
{
	class PacketInPing : public Packet
	{
	private:
		int64_t m_Payload;

	public:
		PacketInPing();

		int64_t GetPayload() const { return m_Payload; }

	protected:
		virtual void Deserialize() override;
	};
}