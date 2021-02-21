#include "Hyperion/HyperPackets/HyperPacketsOut/HyperStatus/PacketOutResponse.h"

namespace Hyperion
{
	PacketOutResponse::PacketOutResponse(const std::string& response)
		: m_Response(response)
	{
	}

	void PacketOutResponse::Serialize()
	{
		WriteVarInt(0x00);
		WriteString(m_Response);
	}
}
