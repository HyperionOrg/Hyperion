#include "Hyperion/HyperPackets/HyperPacketsOut/HyperStatus/PacketOutPong.h"

namespace Hyperion
{
	PacketOutPong::PacketOutPong(int64_t payload)
		: m_Payload(payload)
	{
	}

	void PacketOutPong::Serialize()
	{
		WriteVarInt(0x01);
		WriteInt64(m_Payload);
	}
}
