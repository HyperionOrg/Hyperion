/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Network/Server.hpp>
#include <cstdint>

int32_t main(int32_t, char**)
{
	setbuf(stdout, nullptr);
	
	Server server(25565);
	server.run();

	return 0;
}
