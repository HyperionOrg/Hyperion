#include "PacketOutPong.h"

namespace Hyperion
{
	PacketOutPong::PacketOutPong(int64_t payload)
		: m_Payload(payload)
	{
		Encode();
	}

	void PacketOutPong::Serialize()
	{
		m_Data.push_back(0x01);

		for (size_t i = 0; i < 8; i++)
			m_Data.push_back(static_cast<uint8_t>((m_Payload >> (i * 8)) & 0xFF));
	}
}