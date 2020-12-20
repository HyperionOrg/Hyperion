#pragma once

#include "Network/Packet.h"

namespace Hyperion
{
	class PacketInLogin : public Packet
	{
	private:
		std::string m_Username;

	public:
		PacketInLogin();

		const std::string& GetUsername() const { return m_Username; }

	protected:
		virtual void Deserialize() override;
	};
}