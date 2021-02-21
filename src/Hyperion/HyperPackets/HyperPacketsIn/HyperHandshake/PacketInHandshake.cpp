#include "Hyperion/HyperPackets/HyperPacketsIn/HyperHandshake/PacketInHandshake.h"

namespace Hyperion
{
	PacketInHandshake::PacketInHandshake()
		: m_ProtocolVersion(0), m_ServerAddress(""), m_ServerPort(0), m_NextState(State::STATUS)
	{
	}

	void PacketInHandshake::Deserialize()
	{
		m_ProtocolVersion = ReadVarInt();
		m_ServerAddress = ReadString();
		m_ServerPort = ReadInt16();
		m_NextState = static_cast<State>(ReadInt8());
	}
}
