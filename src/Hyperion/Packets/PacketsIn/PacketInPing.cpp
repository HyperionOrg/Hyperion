#include "PacketInPing.h"

namespace Hyperion
{
	PacketInPing::PacketInPing(const Ref<Packet> packet)
		: Packet(packet), m_Payload(0)
	{
		Decode();
	}

	void PacketInPing::Deserialize()
	{
		size_t j = 7;

		for (size_t i = 0; i < 8; i++)
		{
			m_Payload |= static_cast<uint8_t>(m_Data[i]) << (j * 8);
			j--;
		}
	}
}