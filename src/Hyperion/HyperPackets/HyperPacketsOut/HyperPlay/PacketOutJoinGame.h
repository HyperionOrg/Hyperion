#pragma once

#include "Hyperion/HyperNetwork/Packet.h"
#include "Hyperion/HyperWorld/Gamemode.h"

namespace Hyperion
{
	class PacketOutJoinGame : public Packet
	{
	private:
		int32_t m_Id;
		bool m_Hardcode;
		Gamemode m_Gamemode;
		Gamemode m_PreviousGamemode;

	public:
		PacketOutJoinGame();

		virtual void Serialize() override;
	};
}