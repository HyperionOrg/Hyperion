#pragma once

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#endif

#include <asio.hpp>

#include <string>
#include <thread>
#include <vector>

#include "Core.h"
#include "Network/Client.h"
#include "Network/Connection.h"
#include "Network/Packet.h"
#include "Network/PacketManager.h"
#include "Utilities/ThreadSafeQueue.h"

namespace Hyperion
{
	class HyperionServer
	{
	private:
		uint16_t m_Port;

		Scope<PacketManager> m_PacketManager;

		bool m_Running = true;

		asio::io_context m_Context;
		std::thread m_ContextThread;

		asio::ip::tcp::acceptor m_Acceptor;
		std::deque<Ref<Client>> m_Clients;

		ThreadSafeQueue<OwnedPacket> m_PacketsQueue;

		size_t m_ConnectionCounter = 0;

	public:
		HyperionServer();
		~HyperionServer();

		void Start();
		void Run();

	private:
		void Init();
		void Shutdown();

		void WaitForClients();

		void OnClientConnect(Ref<Client> client);
		void OnClientDisconnect(Ref<Client> client);
	};
}