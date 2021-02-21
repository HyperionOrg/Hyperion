#pragma once

#include <cstdint>

namespace Hyperion
{
	enum class Gamemode : int8_t
	{
		NONE = -1,
		SURVIVAL,
		CREATIVE,
		ADVENTURE,
		SPECTATOR
	};
}