#pragma once

#include "Network/Packet.h"
#include "Utilities/UUID.h"

namespace Hyperion
{
	class PacketOutLoginSuccess : public Packet
	{
	private:
		UUID m_UUID;
		std::string m_Username;

	public:
		PacketOutLoginSuccess(UUID uuid, const std::string& username);

		virtual void Serialize() override;

		UUID GetUUID() const { return m_UUID; }
		const std::string& GetUsername() const { return m_Username; }
	};
}