#pragma once

#include "Hyperion/HyperNetwork/Packet.h"

namespace Hyperion
{
	class PacketInHandshake : public Packet
	{
	public:
		enum class State : uint8_t
		{
			STATUS = 1,
			LOGIN = 2
		};

	private:
		int32_t m_ProtocolVersion;
		std::string m_ServerAddress;
		int16_t m_ServerPort;
		State m_NextState;

	public:
		PacketInHandshake();

		int32_t GetProtocolVersion() const { return m_ProtocolVersion; }
		const std::string& GetServerAddress() const { return m_ServerAddress; }
		int16_t GetServerPort() const { return m_ServerPort; }
		State GetNextState() const { return m_NextState; }

	protected:
		virtual void Deserialize() override;
	};
}