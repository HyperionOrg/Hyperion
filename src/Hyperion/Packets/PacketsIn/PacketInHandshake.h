#pragma once

#include "Packets/Packet.h"

namespace Hyperion
{
	enum class State : uint8_t
	{
		STATUS = 1,
		LOGIN = 2
	};

	class PacketInHandshake : public Packet
	{
	private:
		VarInt m_ProtocolVersion;
		std::string m_ServerAddress;
		uint16_t m_ServerPort;
		State m_State;

	public:
		PacketInHandshake(const Ref<Packet> packet);

		virtual void Deserialize() override;

		VarInt GetProtocolVersion() const { return m_ProtocolVersion; }
		const std::string& GetServerAddress() const { return m_ServerAddress; }
		uint16_t GetServerPort() const { return m_ServerPort; }
		State GetState() const { return m_State; }
	};
}