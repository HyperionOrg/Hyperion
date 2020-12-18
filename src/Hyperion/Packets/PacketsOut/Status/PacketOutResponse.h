#pragma once

#include "Network/Packet.h"

namespace Hyperion
{
	class PacketOutResponse : public Packet
	{
	private:
		std::string m_Response;

	public:
		PacketOutResponse(const std::string& response);

		virtual void Serialize() override;

		const std::string& GetResponse() const { return m_Response; }
	};
}