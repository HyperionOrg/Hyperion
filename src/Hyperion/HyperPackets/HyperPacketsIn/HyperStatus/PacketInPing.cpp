#include "Hyperion/HyperPackets/HyperPacketsIn/HyperStatus/PacketInPing.h"

namespace Hyperion
{
	PacketInPing::PacketInPing()
		: m_Payload(0)
	{
	}

	void PacketInPing::Deserialize()
	{
		m_Payload = ReadInt64();
	}
}
