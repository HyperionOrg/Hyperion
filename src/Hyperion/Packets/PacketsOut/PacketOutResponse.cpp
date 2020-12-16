#include "PacketOutResponse.h"

namespace Hyperion
{
	PacketOutResponse::PacketOutResponse(const std::string& response)
		: m_Response(response)
	{
		Encode();
	}

	void PacketOutResponse::Serialize()
	{
		m_Data.push_back(VarInt(0x00).GetData()[0]);

		VarInt lengthVarInt(static_cast<int32_t>(m_Response.size()));
		for (size_t i = 0; i < lengthVarInt.GetData().size(); i++)
			m_Data.push_back(lengthVarInt.GetData()[i]);

		for (size_t i = 0; i < m_Response.size(); i++)
			m_Data.push_back(static_cast<uint8_t>(m_Response[i]));
	}
}