#include "Hyperion/HyperPackets/HyperPacketsOut/HyperPlay/PacketOutJoinGame.h"

namespace Hyperion
{
	PacketOutJoinGame::PacketOutJoinGame()
	{
	}

	void PacketOutJoinGame::Serialize()
	{
		WriteVarInt(0x24);
	}
}
