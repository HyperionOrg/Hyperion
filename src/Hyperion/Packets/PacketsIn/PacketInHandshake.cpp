#include "PacketInHandshake.h"

namespace Hyperion
{
	PacketInHandshake::PacketInHandshake(const Ref<Packet> packet)
		: Packet(packet), m_ProtocolVersion(0), m_ServerAddress(""), m_ServerPort(0), m_State(State::STATUS)
	{
		Decode();
	}

	void PacketInHandshake::Deserialize()
	{
		m_ProtocolVersion = VarInt::Decode({ m_Data[0], m_Data[1], m_Data[2] });

		for (size_t i = 3; i < m_Data.size() - 3; i++)
			m_ServerAddress += static_cast<char>(m_Data[i]);

		m_ServerPort = static_cast<uint16_t>(m_Data[12]) | m_Data[13];

		m_State = static_cast<State>(m_Data[14]);
	}
}