#include "PacketOutLoginSuccess.h"

namespace Hyperion
{
	PacketOutLoginSuccess::PacketOutLoginSuccess(UUID uuid, const std::string& username)
		: m_UUID(uuid), m_Username(username)
	{
	}

	void PacketOutLoginSuccess::Serialize()
	{
		WriteVarInt(0x02);
		WriteUUID(m_UUID);
		WriteString(m_Username);
	}
}