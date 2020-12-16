#include "PacketInRequest.h"

namespace Hyperion
{
	PacketInRequest::PacketInRequest(const Ref<Packet> packet)
		: Packet(packet)
	{
		Decode();
	}
}