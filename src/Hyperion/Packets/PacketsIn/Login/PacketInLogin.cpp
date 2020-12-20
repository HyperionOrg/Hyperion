#include "PacketInLogin.h"

namespace Hyperion
{
	PacketInLogin::PacketInLogin()
		: m_Username("")
	{
	}

	void PacketInLogin::Deserialize()
	{
		m_Username = ReadString();
	}
}